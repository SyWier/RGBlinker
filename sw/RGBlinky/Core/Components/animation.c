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

const LedFrame_t love[] = {
	{load, { FILL_RGB(255, 0, 255) } },
	{repeat, { 20 } },
	{add, {FILL_RGB(-10, 0, -10)} },
	{repeat, { 20 } },
	{add, {FILL_RGB(10, 0, 10)} },
};

const LedFrame_t black[] = {
	{load, { FILL_RGB(0, 0, 0) } },
};

const LedFrame_t white[] = {
	{load, { FILL_RGB(255, 255, 255) } },
};

const LedFrame_t heart[] = {
	{load, { FILL_RED(63) } },
	{repeat, { 20 } },
	{nope, {} },
	{repeat, { 6 } },
	{add, {FILL_RED(32)} },
	{repeat, { 6 } },
	{add, {FILL_RED(-32)} },
	{repeat, { 6 } },
	{add, {FILL_RED(32)} },
	{repeat, { 6 } },
	{add, {FILL_RED(-32)} },
};

const LedFrame_t hue[] = {
	{load, { FILL_RGB(255, 0, 0) } },
	{repeat, { 17 } },
	{add, {FILL_RGB(0, 15, 0)} },
	{repeat, { 17 } },
	{add, {FILL_RGB(-15, 0, 0)} },
	{repeat, { 17 } },
	{add, {FILL_RGB(0, 0, 15)} },
	{repeat, { 17 } },
	{add, {FILL_RGB(0, -15, 0)} },
	{repeat, { 17 } },
	{add, {FILL_RGB(15, 0, 0)} },
	{repeat, { 17 } },
	{add, {FILL_RGB(0, 0, -15)} },
};

const LedFrame_t hue_circle[] = {
	{load, { 255, 0, 0, 255, 127, 0, 255, 255, 0, 127, 255, 0, 0, 255, 0, 0, 255, 127, 0, 255, 255, 0, 127, 255, 0, 0, 255, 127, 0, 255, 255, 0, 255, 255, 0, 127 } },
	{repeat, { 11 } },
	{shift, {3} },
};

const LedFrame_t amber[] = {
	{load, { FILL_RGB(64, 48, 0) } },
	{add, { RGB(127, 95, 0) } },
	{repeat, { 11 } },
	{shift, {15} },
};

const LedFrame_t blinky[] = {
	{load, { RGB(255, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(255, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
	{repeat, { 35 } },
	{shift, {1} },
};

const LedFrame_t colors[] = {
	{load, { FILL_RED(255) } },
	{load, { FILL_RGB(0, 0, 0) } },
	{load, { FILL_GREEN(255) } },
	{load, { FILL_RGB(0, 0, 0) } },
	{load, { FILL_BLUE(255) } },
	{load, { FILL_RGB(0, 0, 0) } },
};

const LedFrame_t flashes[] = {
	{load, { FILL_RGB(255, 255, 255) } },
	{load, { FILL_RGB(0, 0, 0) } },
};

const LedFrame_t hun[] = {
	{load, { RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255), RGB(0, 255, 0), RGB(0, 255, 0), RGB(0, 255, 0), RGB(0, 255, 0) } },
	{repeat, { 2 } },
	{shift, {12} },
};

const LedFrame_t fairy[] = {
	{load, { RGB(0, 0, 0), RGB(64, 0, 0), RGB(96, 0, 32), RGB(128, 0, 64), RGB(160, 0, 96), RGB(192, 0, 128), RGB(224, 0, 192), RGB(255, 0, 255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
	{repeat, { 11 } },
	{shift, {3} },
};


const LedFrame_t fairy_magic[] = {
	{load, { RGB(255, 0, 255), RGB(192, 0, 128), RGB(128, 0, 64), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(255, 0, 255), RGB(192, 0, 128), RGB(128, 0, 64), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) } },
	{repeat, { 35 } },
	{shift, {-1} },
};

const LedFrame_t magic[] = {
	{load, { RGB(123, 210, 56), RGB(255, 12, 196), RGB(44, 255, 111), RGB(89, 0, 200), RGB(233, 123, 0), RGB(0, 192, 255), RGB(174, 0, 255), RGB(255, 255, 255), RGB(10, 77, 240), RGB(64, 255, 164), RGB(255, 64, 64), RGB(200, 80, 255) }},
	{repeat, { 17 }},
	{shift, {2} },
};

const LedFrame_t chaos[] = {
	{load, { RGB(0, 0, 0), RGB(173, 24, 201), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) }},
	{repeat, { 7 }},
	{shift, {16} },

	{load, { RGB(48, 190, 65), RGB(0, 0, 0), RGB(0, 0, 0), RGB(229, 140, 12), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) }},
	{repeat, { 6 }},
	{shift, {-14} },

	{load, { RGB(0, 0, 0), RGB(255, 194, 64), RGB(0, 0, 0), RGB(101, 16, 255), RGB(0, 0, 0), RGB(225, 36, 143), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) }},
	{repeat, { 5 }},
	{shift, {13} },

	{load, { RGB(181, 0, 241), RGB(0, 255, 195), RGB(98, 0, 0), RGB(0, 0, 0), RGB(255, 199, 46), RGB(0, 0, 0), RGB(234, 14, 255), RGB(0, 0, 0), RGB(55, 210, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) }},
	{repeat, { 4 }},
	{shift, {-11} },

	{load, { RGB(0, 0, 0), RGB(255, 194, 64), RGB(0, 0, 0), RGB(101, 16, 255), RGB(0, 0, 0), RGB(225, 36, 143), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) }},
	{repeat, { 5 }},
	{shift, {13} },

	{load, { RGB(48, 190, 65), RGB(0, 0, 0), RGB(0, 0, 0), RGB(229, 140, 12), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) }},
	{repeat, { 6 }},
	{shift, {-14} },
};

#define ANIMATION(animation, time) animation, (sizeof(animation)/sizeof(animation[0])), time

// Should be minimum 50ms/frame
LedAnimation_t animations[] = {
	{ ANIMATION(love, 50) },
	{ ANIMATION(black, 500) },
	{ ANIMATION(white, 500) },
	{ ANIMATION(fairy, 100) },
	{ ANIMATION(heart, 50) },
	{ ANIMATION(hue_circle, 100) },
	{ ANIMATION(hue, 50) },
	{ ANIMATION(amber, 250) },
	{ ANIMATION(fairy_magic, 40) },
	{ ANIMATION(blinky, 50) },
	{ ANIMATION(chaos, 100) },
	{ ANIMATION(colors, 250) },
	{ ANIMATION(hun, 333) },
	{ ANIMATION(flashes, 100) },
	{ ANIMATION(magic, 100) },
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
		val = -val;

		uint8_t temp[val];

		// Copy first k elements to temp
		memcpy(temp, Animator.pwmBuffer, val);

		// Shift the rest to the left
		memmove(Animator.pwmBuffer, &(Animator.pwmBuffer[val]), LED_CNT - val);

		// Copy temp to the end
		memcpy(&(Animator.pwmBuffer[LED_CNT - val]), temp, val);

		return;
	}
}

void nope(const uint8_t* data) {
	// Nothing is here...
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

