/*
 * animation.h
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#ifndef COMPONENTS_ANIMATION_H_
#define COMPONENTS_ANIMATION_H_ű

#include "stdbool.h"

extern bool animation_debug_mode;

void Animation_Debug_Handle();
void Animation_Debug_Next();
void Animation_Debug_Prev();

void Animation_Handle();
void Animation_Next();
void Animation_Prev();

#endif /* COMPONENTS_ANIMATION_H_ */
