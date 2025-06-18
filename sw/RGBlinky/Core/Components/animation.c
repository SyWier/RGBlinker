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
//
//void Animation_Debug_Handle() {
//	switch(animation_debug_state) {
//	case 0: Led_Test(0x1240); break; // Red
//	case 1: Led_Test(0x0920); break; // Green
//	case 2: Led_Test(0x0490); break; // Blue
//	default: Led_Test(0x1FF0); break; // White - Unknown state
//	}
//}
//
//void Animation_Debug_Next() {
//	switch(animation_debug_state) {
//	case 0: animation_debug_state = 1; break;
//	case 1: animation_debug_state = 2; break;
//	case 2: animation_debug_state = 0; break;
//	default: animation_debug_state = 0; break;
//	}
//}
//
//void Animation_Debug_Prev() {
//	switch(animation_debug_state) {
//	case 0: animation_debug_state = 2; break;
//	case 1: animation_debug_state = 0; break;
//	case 2: animation_debug_state = 1; break;
//	default: animation_debug_state = 0; break;
//	}
//}
//
//void Animation_Handle() {
//	// Debug mode - stationary leds to identify faulty LEDs
//	if(animation_debug_mode) {
//		Animation_Debug_Handle();
//		return;
//	}
//
//	// Normal mode
//	switch(animation_state) {
//	case 0: Led_Test(0x1B60); break; // Yellow
//	case 1: Led_Test(0x0DB0); break; // Cyan
//	case 2: Led_Test(0x16D0); break; // Magenta
//	default: Led_Test(0x1FF0); break; // White - Unknown state
//	}
//}

void Animation_Next() {
	Animator.lastTime = 0;
	Animator.frameIndex = 0;
	Animator.repeatCount = 0;
	Animator.animationIndex++;
	if(Animator.animationIndex >= Animator.animationCount) {
		Animator.animationIndex = 0;
	}
}
void Animation_Prev() {
	Animator.lastTime = 0;
	Animator.frameIndex = 0;
	Animator.repeatCount = 0;
	if(Animator.animationIndex == 0) {
		Animator.animationIndex = Animator.animationCount;
	}
	Animator.animationIndex--;
}

const LedFrame_t frames1[] = {
		{load, { RGB(255, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(255, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
		{repeat, { 35 } },
		{shift, {1} },
};

const LedFrame_t frames2[] = {
		{load, { FILL_RED(255) } },
		{load, { FILL_GREEN(255) } },
		{load, { FILL_BLUE(255) } },
		{load, { FILL_WHITE(255) } },
};

const LedFrame_t frames3[] = {
		{load, { FILL_RGB(255, 0, 255) } },
		{repeat, { 15 } },
		{add, {FILL_RGB(-15, 0, -15)} },
		{repeat, { 15 } },
		{add, {FILL_RGB(15, 0, 15)} },
};


#define LEN(var) ( sizeof(var)/sizeof(var[0]) )

LedAnimation_t animations[] = {
		{frames1, LEN(frames1), 50},
		{frames2, LEN(frames2), 500},
		{frames3, LEN(frames3), 100},
};

LedAnimator_t Animator = {
		.pwmBuffer = {},

		.frameIndex = 0,
		.animationIndex = 0,

		.animations = animations,
		.animationCount = sizeof(animations)/sizeof(LedAnimation_t),

		.lastTime = 0,
		.repeatCount = 0,
};

bool animationFlag = 0;;

void load(const uint8_t* data) {
	memcpy(Animator.pwmBuffer, data, LED_CNT);
//	Log_Info("load: %d", Animator.pwmBuffer[0]);
}
void add(const uint8_t* data) {
	for(int i = 0; i < LED_CNT; i++) {
		Animator.pwmBuffer[i] += (int8_t)data[i];
	}
}

void repeat(const uint8_t* data) {
	Animator.lastTime = 0;
	Animator.repeatCount = data[0] - 1;
}

void shift(const uint8_t* data) {
	int8_t val = (int8_t)data[0];

	if(val > 0) {
//		Log_Debug("(Instr) shift right: %d", data[0]);

		uint8_t temp[val];

		// Copy last k elements to temp
		memcpy(temp, &Animator.pwmBuffer[LED_CNT - val], val);

		// Shift the rest to the right
		memmove(&Animator.pwmBuffer[val], Animator.pwmBuffer, LED_CNT - val);

		// Copy temp to the front
		memcpy(Animator.pwmBuffer, temp, val);

		return;
	}

	if(val < 0) {
//		Log_Debug("(Instr) shift left: %d", data[0]);

		val = -val;

		uint8_t temp[val];

		// Copy first k elements to temp
		memcpy(temp, Animator.pwmBuffer, val);

		// Shift the rest to the left
		memmove(Animator.pwmBuffer, &Animator.pwmBuffer[val], LED_CNT - val);

		// Copy temp to the end
		memcpy(&Animator.pwmBuffer[LED_CNT - val], temp, val);

		return;
	}
}

void Animate() {
	// Return if no animation is running
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
	Led_Generate_Buffer(Animator.pwmBuffer);

	// Check repeat
	if(frame.instruction != repeat && Animator.repeatCount > 0) {
		Animator.repeatCount--;
		return;
	}

	// Update index
	Animator.frameIndex++;
	if(Animator.frameIndex >= animation.frameCount) {
		Animator.frameIndex = 0;
	}
}

