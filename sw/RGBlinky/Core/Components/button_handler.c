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
#include "animation.h"
#include "battery.h"

#define BTN_DEBOUNCE_MS	50		// Ignore contact bounce
#define BTN_DOUBLE_MS	200     // Max gap before 2nd click
#define BTN_LONG_MS		2000    // Press ≥ BTN_LONG_MS s
#define BTN_EXTRA_MS	3000    // Press ≥ BTN_EXTRA_MS + BTN_LONG_MS s

Button userBtn;
bool animationDebugMode = false;

const uint8_t white[] = { FILL_RGB(255, 255, 255) };

static inline uint8_t btn_level(Button *b) {
	return HAL_GPIO_ReadPin(b->port, b->pin);  // assumes active-low push-button
}
static inline uint8_t btn_elapsed(Button *b, uint32_t duration_ms) {
	if( (HAL_GetTick() - b->t_stamp) < duration_ms) {
		return 0;
	} else {
		return 1;
	}
}
static inline void btn_set_state(Button *b, btn_state_t new_state, const char* msg) {
    Log_Debug(msg);
    b->state = new_state;
    b->t_stamp = HAL_GetTick();
}


void btn_single_click() {
	if(animationDebugMode == 0) {
		Animation_Next();
	} else {
		if(RgbBrigntnessScale == 0) {
			RgbBrigntnessScale = 5;
		}
		RgbBrigntnessScale--;
		Led_Generate_Buffer(white);
	}
}
void btn_double_click() {
	if(animationDebugMode == 0) {
		Animation_Prev();
	} else {
		RgbBrigntnessScale++;
		if(RgbBrigntnessScale >= 5) {
			RgbBrigntnessScale = 0;
		}
		Led_Generate_Buffer(white);
	}
}

void btn_single_to_long() {
	 // Turn off all LEDs
	animationFlag = 0;
	Led_Fill_Buffer(0x1FFF);
}
void btn_long_to_extra() {
	// Turn on all LEds
	Led_Generate_Buffer(white);
//	Led_Test(0x1FF0);
}
void btn_long_press() {
	PowerOff();
}
void btn_extra_press() {
	animationDebugMode = !animationDebugMode;
	animationFlag = !animationDebugMode;
}

void Button_Init(Button *b) {
	b->port = BTN_GPIO_Port;
	b->pin = BTN_Pin;
	b->state = BTN_BOOT;
	b->is_initialized = 1;
}

/* ===================================================================================== *
 * 													+----------+	+--------+			 *
 * 										(idle)	<-	| RELEASED | -> | DOUBLE | -> (idle) *
 * 													+----------+	+--------+			 *
 *	+--------+		+--------+		+--------+	 /										 *
 *	|  BOOT  |  ->	|  IDLE  |  ->  | SINGLE | --										 *
 *	+--------+		+--------+		+--------+	 \										 *
 * 													+--------+		+--------+			 *
 * 										(idle)	<-	|  LONG  |  ->	| EXTRA  | -> (idle) *
 * 													+--------+		+--------+			 *
 * ===================================================================================== */

void Button_Tick(Button *b) {
	if(b->is_initialized == 0) {
		return;
	}

	switch(b->state) {
	case BTN_BOOT:
		if(!btn_level(b)) {
			btn_set_state(b, BTN_IDLE, "(btn) BOOT to IDLE");
		}

		break;
	case BTN_IDLE:
		// Wait for debounce
		if(!btn_elapsed(b, BTN_DEBOUNCE_MS)) {
			break;
		}

		// IDLE to SINGLE
		if(btn_level(b)) {
			btn_set_state(b, BTN_SINGLE, "(btn) IDLE to SINGLE");
			break;
		}

		break;
	case BTN_SINGLE:
		// Wait for debounce
		if(!btn_elapsed(b, BTN_DEBOUNCE_MS)) {
			break;
		}

		// SINGLE to RELEASED
		if (!btn_level(b)) {
			btn_set_state(b, BTN_RELEASED, "(btn) SINGLE to RELEASED");
			break;
		}

		// SINGLE to LONG
		if(btn_elapsed(b, BTN_LONG_MS)) {
			btn_single_to_long();
			btn_set_state(b, BTN_LONG, "(btn) SINGLE to LONG");
			break;
		}

		break;
	case BTN_LONG:
		// LONG to IDLE
		if (!btn_level(b)) {
			btn_long_press();
			btn_set_state(b, BTN_IDLE, "(btn) LONG to IDLE");
			break;
		}

		// LONG to EXTRA
		if(btn_elapsed(b, BTN_EXTRA_MS)) {
			btn_long_to_extra();
			btn_set_state(b, BTN_EXTRA, "(btn) LONG to EXTRA");
			break;
		}

		break;
	case BTN_EXTRA:
		// EXTRA to IDLE
		if (!btn_level(b)) {
			btn_extra_press();
			btn_set_state(b, BTN_IDLE, "(btn) EXTRA to IDLE");
			break;
		}

		break;
	case BTN_RELEASED:
		// Wait for debounce
		if(!btn_elapsed(b, BTN_DEBOUNCE_MS)) {
			break;
		}

		// RELEASED to DOUBLE
		// Button is pressed a second time -> double click
		if (btn_level(b)) {
			btn_double_click();
			btn_set_state(b, BTN_DOUBLE, "(btn) RELEASED to DOUBLE");
			break;
		}

		// RELEASED to IDLE
		// ~ Single click
		if(btn_elapsed(b, BTN_DOUBLE_MS)) {
			btn_single_click();
			btn_set_state(b, BTN_IDLE, "(btn) RELEASED to IDLE");
			break;
		}

		break;
	case BTN_DOUBLE:
		// Wait for debounce
		if(!btn_elapsed(b, BTN_DEBOUNCE_MS)) {
			break;
		}

		// DOUBLE to IDLE
		if (!btn_level(b)) {
			btn_set_state(b, BTN_IDLE, "(btn) DOUBLE to IDLE");
			break;
		}

		break;
	default:
		Log_Error("(btn) UNKNOWN State!");
		b->state = BTN_IDLE;
		b->t_stamp = HAL_GetTick();
		break;
	}
}
