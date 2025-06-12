/*
 * animation.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#include "animation.h"
#include "led.h"

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
