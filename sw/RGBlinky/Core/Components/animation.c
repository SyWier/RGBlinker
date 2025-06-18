/*
 * animation.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#include "animation.h"
#include "led.h"
#include "log.h"
#include <string.h>

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
	Animator.lastTime = 0;
	Animator.frameIndex = 0;
	Animator.animationIndex++;
	if(Animator.animationIndex >= Animator.animationCount) {
		Animator.animationIndex = 0;
	}
}
void Animation_Prev() {
	Animator.lastTime = 0;
	Animator.frameIndex = 0;
	if(Animator.animationIndex == 0) {
		Animator.animationIndex = Animator.animationCount;
	}
	Animator.animationIndex--;
}

const LedFrame_t frames1[] = {
		{load, { RGB(255, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
		{load, { RGB(0, 0, 0), RGB(0, 255, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
		{load, { RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
};

const LedFrame_t frames2[] = {
		{load, { FILL_RED(255) } },
		{load, { FILL_GREEN(255) } },
		{load, { FILL_BLUE(255) } },
		{load, { FILL_WHITE(255) } },
};

const LedFrame_t frames3[] = {
		{load, { FILL_RED(255) } },
		{load, { FILL_RED(196) } },
		{load, { FILL_RED(128) } },
		{load, { FILL_RED(64) } },
};


#define LEN(var) ( sizeof(var)/sizeof(var[0]) )

LedAnimation_t animations[] = {
		{frames1, LEN(frames1), 250},
		{frames2, LEN(frames2), 500},
		{frames3, LEN(frames3), 250},
};

LedAnimator_t Animator = {
		.frameIndex = 0,
		.animationIndex = 0,

		.animations = animations,
		.animationCount = sizeof(animations)/sizeof(LedAnimation_t),

		.lastTime = 0,
		.repeatCount = 0,
};

bool animationFlag = 0;;

void load(const uint8_t* data) {
	Led_Generate_Buffer(data);
}
void add(const uint8_t* data) {
	signed char value;
	for(int i = 0; i < LED_CNT; i++) {
		value = data[i];

	}
}
void repeat(const uint8_t* data) {
	Log_Debug("(Instr) repeat: %d", data[0]);
}

void shift(const uint8_t* data) {
	Log_Debug("(Instr) shift: %d", data[0]);
}

void Animate() {
	// Return
	if(animationFlag == 0) {
		return;
	}

	// Get handles
	LedAnimation_t animation = Animator.animations[Animator.animationIndex];
	LedFrame_t frame = animation.frames[Animator.frameIndex];

	// Check ellapsed time
	if(HAL_GetTick() - Animator.lastTime < animation.frameTime) {
		return;
	}
	Animator.lastTime = HAL_GetTick();

	// Handle animation
	frame.instruction(frame.data);

	// Update index
	Animator.frameIndex++;
	if(Animator.frameIndex >= animation.frameCount) {
		Animator.frameIndex = 0;
	}
}

