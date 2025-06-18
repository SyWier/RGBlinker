/*
 * led.h
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#ifndef COMPONENTS_LED_H_
#define COMPONENTS_LED_H_ű

#include "main.h"

#define LED_ROWS 4
#define LED_NUM 9
#define LED_CNT LED_ROWS * LED_NUM
#define LED_PWM_MAX 63 // 0...15 -> 16 steps
#define BUFFER_SIZE LED_ROWS * LED_PWM_MAX
#define BUFFER_COUNT 2

extern uint16_t LedBuffer[BUFFER_COUNT][BUFFER_SIZE + 4]; // Size = BUFFER_COUNT * BUFFER_SIZE * 2 byte
extern bool BufferSelect;

#define RGB_SCALE 2
#define RGB(r, g, b)  (b >> RGB_SCALE), (g >> RGB_SCALE), (r >> RGB_SCALE)

void Led_Init();
void Led_Fill_Buffer(uint16_t color);
void Led_Generate_Buffer(uint8_t frame[LED_CNT]);
void Led_Test(uint32_t colorRaw);

#endif /* COMPONENTS_LED_H_ */
