/*
 * animation.h
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#ifndef COMPONENTS_ANIMATION_H_
#define COMPONENTS_ANIMATION_H_ű

#include "main.h"
#include "stdbool.h"
#include "led.h"

extern bool animation_debug_mode;

void Animation_Debug_Handle();
void Animation_Debug_Next();
void Animation_Debug_Prev();

void Animation_Handle();
void Animation_Next();
void Animation_Prev();

typedef void (*instructions_t)(const uint8_t* data);

typedef struct {
	const instructions_t instruction;
	const uint8_t data[LED_CNT];
} LedFrame_t;

typedef struct {
	const LedFrame_t* frames;
	const uint8_t frameCount;
	const uint16_t frameTime;
} LedAnimation_t;

typedef struct {
	uint8_t pwmBuffer[LED_CNT];

	uint8_t frameIndex;
	uint8_t animationIndex;

	LedAnimation_t* animations;
	uint8_t animationCount;

	uint32_t lastTime;
	uint8_t repeatCount;
} LedAnimator_t;

extern LedAnimator_t Animator;
extern bool animationFlag;

void load(const uint8_t* data);
void add(const uint8_t* data);
void repeat(const uint8_t* data);
void shift(const uint8_t* data);
void nope(const uint8_t* data);

void Animate();

#endif /* COMPONENTS_ANIMATION_H_ */
