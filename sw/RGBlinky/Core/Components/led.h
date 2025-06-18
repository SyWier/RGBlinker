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


#define RGB_BUFFER_SCALE 2
#define RGB_BRIGHTNESS_SCALE 1
#define RGB_SCALE (RGB_BUFFER_SCALE + RGB_BRIGHTNESS_SCALE)

#ifdef HW_V1_0
#define RGB(r, g, b)  (b >> RGB_SCALE), (g >> RGB_SCALE), (r >> RGB_SCALE)
#elif defined(HW_V1_1)
#define RGB(r, g, b)  (r >> RGB_SCALE), (g >> RGB_SCALE), (b >> RGB_SCALE)
#endif

#define FILL_RGB(r, g, b) \
    RGB(r, g, b), RGB(r, g, b), RGB(r, g, b), RGB(r, g, b), \
    RGB(r, g, b), RGB(r, g, b), RGB(r, g, b), RGB(r, g, b), \
    RGB(r, g, b), RGB(r, g, b), RGB(r, g, b), RGB(r, g, b)

#define FILL_RED(val)     FILL_RGB(val, 0, 0)
#define FILL_GREEN(val)   FILL_RGB(0, val, 0)
#define FILL_BLUE(val)    FILL_RGB(0, 0, val)
#define FILL_WHITE(val)   FILL_RGB(val, val, val)

void Led_Init();
void Led_Fill_Buffer(uint16_t color);
void Led_Generate_Buffer(uint8_t frame[LED_CNT]);
void Led_Test(uint32_t colorRaw);

#endif /* COMPONENTS_LED_H_ */
