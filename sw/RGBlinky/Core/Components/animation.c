/*
 * animation.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#include "animation.h"
#include "led.h"
#include "log.h"

bool animation_debug_mode = 0;
uint8_t animation_debug_state = 0;
uint8_t animation_state = 0;

void Animation_Debug_Handle() {
	switch(animation_debug_state) {
	case 0: Led_Test(0x1240); break; // Red
	case 1: Led_Test(0x0920); break; // Green
	case 2: Led_Test(0x0490); break; // Blue
	default: Led_Test(0x1FF0); break; // White - Unknown state
	}
}

void Animation_Debug_Next() {
	switch(animation_debug_state) {
	case 0: animation_debug_state = 1; break;
	case 1: animation_debug_state = 2; break;
	case 2: animation_debug_state = 0; break;
	default: animation_debug_state = 0; break;
	}
}

void Animation_Debug_Prev() {
	switch(animation_debug_state) {
	case 0: animation_debug_state = 2; break;
	case 1: animation_debug_state = 0; break;
	case 2: animation_debug_state = 1; break;
	default: animation_debug_state = 0; break;
	}
}

void Animation_Handle() {
	// Debug mode - stationary leds to identify faulty LEDs
	if(animation_debug_mode) {
		Animation_Debug_Handle();
		return;
	}

	// Normal mode
	switch(animation_state) {
	case 0: Led_Test(0x1B60); break; // Yellow
	case 1: Led_Test(0x0DB0); break; // Cyan
	case 2: Led_Test(0x16D0); break; // Magenta
	default: Led_Test(0x1FF0); break; // White - Unknown state
	}
}

void Animation_Next() {
	if(animation_debug_mode) {
		Animation_Debug_Next();
		return;
	}

	switch(animation_state) {
	case 0: animation_state = 1; break;
	case 1: animation_state = 2; break;
	case 2: animation_state = 0; break;
	default: animation_state = 0; break;
	}
}
void Animation_Prev() {
	if(animation_debug_mode) {
		Animation_Debug_Prev();
		return;
	}

	switch(animation_state) {
	case 0: animation_state = 2; break;
	case 1: animation_state = 0; break;
	case 2: animation_state = 1; break;
	default: animation_state = 0; break;
	}
}

void Anim_Load(uint8_t* buffer) {

}
void Anim_Add(uint8_t* buffer, uint8_t* pattern) {

}
void Anim_Shift(uint8_t* buffer, uint8_t cnt) {
	uint8_t LUT[12] = {1, 12, 9, 6, 3, 11, 8, 5, 2, 10, 7, 4};

	uint8_t tmp[LED_CNT];
	memcpy(tmp, buffer, LED_CNT);

	for(int i = 0; i < LED_CNT; i++) {
		buffer[i] = buffer[i-1];
	}
	buffer[0] = tmp;
}

void Anim_1() {
	uint8_t frame[LED_CNT] = {
		RGB(255, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
		RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
		RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0)
	};

//	for(int i = 0; i < LED_CNT; i+=3) {
//		frame[i] = 63;
//	}

	Led_Generate_Buffer(frame);

//	for(int i = 0; i < 12; i++) {
	while(1) {
		Anim_Shift(frame, 1);
		Led_Generate_Buffer(frame);
		HAL_Delay(1000);
	}

}
void Anim_2() {

}

const LedFrame_t anim1[] = {
		{load, { RGB(255, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
		{load, { RGB(0, 0, 0), RGB(0, 255, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
		{load, { RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
		{load, { FILL_RED(255) } },
		{load, { FILL_RED(196) } },
		{load, { FILL_RED(128) } },
		{load, { FILL_RED(64) } },
		{load, { FILL_GREEN(255) } },
		{load, { FILL_BLUE(255) } },
		{load, { FILL_WHITE(255) } },
		{repeat, {5} },
};

void load(uint8_t* data) {
	Log_Debug("(Instr) load");
	Led_Generate_Buffer(data);
}
void add(uint8_t* data) {
	Log_Debug("(Instr) add");
}
void repeat(uint8_t* data) {
	Log_Debug("(Instr) repeat: %d", data[0]);
}

