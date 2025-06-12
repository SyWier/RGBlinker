/*
 * button_handler.c
 *
 *  Created on: Jun 9, 2025
 *      Author: Daniel
 */

#include "button_handler.h"
#include "log.h"
#include "led.h"
#include "main.h"

Button userBtn;

static inline uint8_t btn_level(Button *b) {
	return HAL_GPIO_ReadPin(b->port, b->pin);  // assumes active-low push-button
}

void btn_single_click(void) {
	Log_Debug("Single");
	Led_Test(0x1240);
//	GPIOA->ODR = 0x1FFF;
//	HAL_GPIO_WritePin(EN_3V3_GPIO_Port, EN_3V3_Pin, 1); // Turn on 3.3V
}
void btn_double_click(void) {
	Log_Debug("Double");
	Led_Test(0x0920);
//	GPIOA->ODR = 0x1FFF;
//	HAL_GPIO_WritePin(EN_3V3_GPIO_Port, EN_3V3_Pin, 1); // Turn on 3.3V
}
void btn_long_press(void) {
	Log_Debug("Long");
	Log_Important("Going to standby mode... zzz...");
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN3_HIGH);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF3);
	HAL_PWR_EnterSTANDBYMode();
}
void btn_vlong_press(void) {
	Log_Debug("Very long");
	Led_Test(0x1FF0);
//	GPIOA->ODR = 0x1FFF;
//	HAL_GPIO_WritePin(EN_3V3_GPIO_Port, EN_3V3_Pin, 1); // Turn on 3.3V
}

void Button_Init(Button *b) {
	b->port = BTN_GPIO_Port;
	b->pin = BTN_Pin;
	b->state = BTN_BOOT;
	b->action = btn_single_click;
	b->is_initialized = 1;
}

void Button_Tick(Button *b) {
	if(b->is_initialized == 0) {
		return;
	}

	if(b->t_stamp < 16565) {
		b->t_stamp++;
	}

	switch (b->state) {
	case BTN_BOOT:
		if (!btn_level(b)) {
			b->state = BTN_IDLE;
			b->t_stamp = 0;
		}
		break;
	case BTN_IDLE:
		//Wait for debounce
		if (b->t_stamp < BTN_DEBOUNCE_MS) {
			break;
		}

		if (btn_level(b)) {
			b->state = BTN_PRESSED;
			b->t_stamp = 0;
			b->action = btn_single_click;
			break;
		}

		break;
	case BTN_PRESSED:
		//Wait for debounce
		if (b->t_stamp < BTN_DEBOUNCE_MS) {
			break;
		}

		// Check if it is a long press
		if (b->t_stamp > BTN_LONG_MS) {
			Led_Fill_Buffer(0x1FFF);
			b->action = btn_long_press;
		}

		// Check if it is a very long press
		if (b->t_stamp > BTN_VLONG_MS) {
			b->action = btn_vlong_press;
		}

		// Check if button is released
		if (!btn_level(b)) {
			b->state = BTN_RELEASED;
			b->t_stamp = 0;
			break;
		}

		break;

	case BTN_RELEASED:
		//Wait for debounce
		if (b->t_stamp < BTN_DEBOUNCE_MS) {
			break;
		}

		// Button is pressed a second time -> double click
		if (btn_level(b)) {
			b->state = BTN_DOUBLE;
			b->t_stamp = 0;
			b->action = btn_double_click;
			break;
		}

		// Button is released -> action
		if (b->t_stamp > BTN_DOUBLE_MS) {
			b->state = BTN_IDLE;
			b->t_stamp = 0;
			b->action();
			break;
		}

		break;
	case BTN_DOUBLE:
		//Wait for debounce
		if (b->t_stamp < BTN_DEBOUNCE_MS) {
			break;
		}

		// Check if button is released
		if (!btn_level(b)) {
			b->state = BTN_IDLE;
			b->t_stamp = 0;
			b->action();
			break;
		}

		break;
	}
}
