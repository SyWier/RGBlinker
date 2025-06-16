/*
 * led.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#include "led.h"

uint16_t LedBuffer[BUFFER_COUNT][BUFFER_SIZE + 4];
bool BufferSelect = 0;

void Led_Init() {
	// Init LED buffer (turn off all LEDs by default))
	// The last values are unused for optimalization reasons
	uint16_t reg = GPIOA->ODR | 0x1FFF;
	for (uint16_t i = 0; i < BUFFER_SIZE + 4; i++) {
		LedBuffer[0][i] = reg;
		LedBuffer[1][i] = reg;
	}

	HAL_GPIO_WritePin(EN_3V3_GPIO_Port, EN_3V3_Pin, 1); // Turn on 3.3V
}

void Led_Fill_Buffer(uint16_t color) {
	for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
		LedBuffer[0][i] = (GPIOA->ODR & ~0x1FFF) | (color & 0x1FFF); // Write back only PA0–PA12
		LedBuffer[1][i] = (GPIOA->ODR & ~0x1FFF) | (color & 0x1FFF); // Write back only PA0–PA12
	}
}

// Macros for buffer generation
#define ANODE_PIN(row)     (1U << (row))
#define CATHODE_PIN(col)   (1U << ((col) + LED_ROWS))

void Led_Generate_Buffer(uint8_t frame[LED_CNT]) {
	// Delete buffer (keep upper bits)
	// LED anode, cathode HIGH -> Turn OFF
	uint16_t reg = GPIOA->ODR | 0x1FFF;
	for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
		LedBuffer[BufferSelect][i] = reg;
	}

	// Generate buffer
	for (uint16_t row = 0; row < LED_ROWS; row++) { // Select row
		for (uint16_t led = 0; led < LED_NUM; led++) { // Select LED

			// PWM number between 0-255
//			uint8_t pwm = gamma_lut[frame[row * LED_NUM + led]];
			uint8_t pwm = frame[row * LED_NUM + led];
			if (pwm > LED_PWM_MAX) {
				pwm = LED_PWM_MAX;
			}

			// Set LED values
			for (uint16_t n = 0; n < pwm; n++) {
				LedBuffer[BufferSelect][row * LED_PWM_MAX + n] &= ~ANODE_PIN(row); // LED anode
				LedBuffer[BufferSelect][row * LED_PWM_MAX + n] &= ~CATHODE_PIN( led); // LED cathode
			}
		}
	}

	BufferSelect = !BufferSelect;

	return;
}

void Led_Test(uint32_t colorRaw) {
	uint32_t reg; // Temporary buffer to calculate output

	for(uint32_t i = 0; i < 4; i++) {

		reg = 0x1FFF;				// Set all 13 bits high (anodes/cathodes OFF)
		reg &= ~(1 << i);           // Set selected anode HIGH (set P-MOSFET gate LOW)
		reg &= ~colorRaw;           // Set selected cathode LOW (sink current)

		for (uint16_t j = 0; j < LED_PWM_MAX; j++) {
			LedBuffer[BufferSelect][i * LED_PWM_MAX + j] = (GPIOA->ODR & ~0x1FFF) | (reg & 0x1FFF); // Write back only PA0–PA12
		}
	}

	BufferSelect = !BufferSelect;
}
