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

void Anim_Load(uint8_t* buffer);
void Anim_Add(uint8_t* buffer, uint8_t* pattern);
void Anim_Shift(uint8_t* buffer, uint8_t cnt);

typedef void (*instructions_t)(uint8_t* data);

typedef struct {
	instructions_t instruction;
	uint8_t data[LED_CNT];
} LedFrame_t;

typedef struct {
	LedFrame_t* frames;
	uint8_t frameCount;
	uint16_t frameTime;
} LedAnimation_t;

typedef struct {
	uint8_t frameIndex;
	uint8_t animationIndex;

	uint8_t animationCount;
	LedAnimation_t* animations;

	uint32_t lastTime;
	uint8_t repeatCount;
} LedAnimator_t;

extern const LedFrame_t anim1[4];

void load(uint8_t* data);
void add(uint8_t* data);
void repeat(uint8_t* data);
void shift(uint8_t* data);

#endif /* COMPONENTS_ANIMATION_H_ */
