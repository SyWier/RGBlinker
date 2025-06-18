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

void Anim_1();
void Anim_2();



#endif /* COMPONENTS_ANIMATION_H_ */
