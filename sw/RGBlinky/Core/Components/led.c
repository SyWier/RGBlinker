/*
 * led.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#include "led.h"

uint16_t LedBuffer[BUFFER_COUNT][BUFFER_SIZE + 4];
bool BufferSelect = 0;

// Gamma brightness lookup table <https://victornpb.github.io/gamma-table-generator>
// gamma = 1.50 steps = 256 range = 0-255
const uint8_t gamma_lut[256] = {
     0,   0,   0,   0,   1,   1,   1,   1,   1,   2,   2,   2,   3,   3,   3,   4,
     4,   4,   5,   5,   6,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,  11,
    11,  12,  12,  13,  14,  14,  15,  15,  16,  16,  17,  18,  18,  19,  20,  20,
    21,  21,  22,  23,  23,  24,  25,  26,  26,  27,  28,  28,  29,  30,  31,  31,
    32,  33,  34,  34,  35,  36,  37,  37,  38,  39,  40,  41,  41,  42,  43,  44,
    45,  46,  46,  47,  48,  49,  50,  51,  52,  53,  53,  54,  55,  56,  57,  58,
    59,  60,  61,  62,  63,  64,  65,  65,  66,  67,  68,  69,  70,  71,  72,  73,
    74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  88,  89,  90,
    91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 102, 103, 104, 105, 106, 107,
   108, 109, 110, 112, 113, 114, 115, 116, 117, 119, 120, 121, 122, 123, 124, 126,
   127, 128, 129, 130, 132, 133, 134, 135, 136, 138, 139, 140, 141, 142, 144, 145,
   146, 147, 149, 150, 151, 152, 154, 155, 156, 158, 159, 160, 161, 163, 164, 165,
   167, 168, 169, 171, 172, 173, 174, 176, 177, 178, 180, 181, 182, 184, 185, 187,
   188, 189, 191, 192, 193, 195, 196, 197, 199, 200, 202, 203, 204, 206, 207, 209,
   210, 211, 213, 214, 216, 217, 218, 220, 221, 223, 224, 226, 227, 228, 230, 231,
   233, 234, 236, 237, 239, 240, 242, 243, 245, 246, 248, 249, 251, 252, 254, 255,
};

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

void Led_Generate_Buffer(const uint8_t frame[LED_CNT]) {
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
			pwm = gamma_lut[pwm];
			pwm = pwm >> RGB_SCALE;
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
