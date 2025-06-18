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

void Anim_Load(uint8_t* buffer) {

}
void Anim_Add(uint8_t* buffer, uint8_t* pattern) {

}
void Anim_Shift(uint8_t* buffer, uint8_t cnt) {
	uint8_t LUT[12] = {1, 12, 9, 6, 3, 11, 8, 5, 2, 10, 7, 4};

	uint8_t tmp[LED_CNT];
	memcpy(tmp, buffer, LED_CNT);

	for(int i = 0; i < LED_CNT; i++) {
		buffer[i] = buffer[i-1];
	}
	buffer[0] = tmp;
}

void Anim_1() {
	uint8_t frame[LED_CNT] = {
		RGB(255, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
		RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
		RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0)
	};

//	for(int i = 0; i < LED_CNT; i+=3) {
//		frame[i] = 63;
//	}

	Led_Generate_Buffer(frame);

//	for(int i = 0; i < 12; i++) {
	while(1) {
		Anim_Shift(frame, 1);
		Led_Generate_Buffer(frame);
		HAL_Delay(1000);
	}

}
void Anim_2() {

}

// Gamma brightness lookup table <https://victornpb.github.io/gamma-table-generator>
// gamma = 1.50 steps = 256 range = 0-255
const uint8_t gamma_lut[256] = {
     0,   0,   0,   0,   1,   1,   1,   1,   1,   2,   2,   2,   3,   3,   3,   4,
     4,   4,   5,   5,   6,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,  11,
    11,  12,  12,  13,  14,  14,  15,  15,  16,  16,  17,  18,  18,  19,  20,  20,
    21,  21,  22,  23,  23,  24,  25,  26,  26,  27,  28,  28,  29,  30,  31,  31,
    32,  33,  34,  34,  35,  36,  37,  37,  38,  39,  40,  41,  41,  42,  43,  44,
    45,  46,  46,  47,  48,  49,  50,  51,  52,  53,  53,  54,  55,  56,  57,  58,
    59,  60,  61,  62,  63,  64,  65,  65,  66,  67,  68,  69,  70,  71,  72,  73,
    74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  88,  89,  90,
    91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 102, 103, 104, 105, 106, 107,
   108, 109, 110, 112, 113, 114, 115, 116, 117, 119, 120, 121, 122, 123, 124, 126,
   127, 128, 129, 130, 132, 133, 134, 135, 136, 138, 139, 140, 141, 142, 144, 145,
   146, 147, 149, 150, 151, 152, 154, 155, 156, 158, 159, 160, 161, 163, 164, 165,
   167, 168, 169, 171, 172, 173, 174, 176, 177, 178, 180, 181, 182, 184, 185, 187,
   188, 189, 191, 192, 193, 195, 196, 197, 199, 200, 202, 203, 204, 206, 207, 209,
   210, 211, 213, 214, 216, 217, 218, 220, 221, 223, 224, 226, 227, 228, 230, 231,
   233, 234, 236, 237, 239, 240, 242, 243, 245, 246, 248, 249, 251, 252, 254, 255,
};



//typedef enum {
//	LOAD, ADD, REPEAT, SHIFT
//} LED_OPCODE_E;
//
//typedef struct {
//	uint8_t red;
//	uint8_t green;
//	uint8_t blue;
//} LedRgb_t;
//typedef struct {
//	uint16_t frameTime;
//	LedRgb_t brightness[ROWS][COLS];
//} LedFrame_t;
//typedef struct {
//	LED_OPCODE_E opcode;
//	LedFrame_t frame;
//} LedAnimation_t;
//
//LedAnimation_t animation[] = { LOAD, { 100, { { { 255, 0, 0 }, { 0, 255, 0 }, {
//		0, 0, 255 } }, { { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 } }, { { 255,
//		0, 0 }, { 0, 255, 0 }, { 0, 0, 255 } }, { { 255, 0, 0 }, { 0, 255, 0 },
//		{ 0, 0, 255 } } } }, REPEAT, };
//
//int leds[12];
//
//int instCnt = 0;
//int instMax = 3;
//typedef void (*instructions_t)(void);
//void load() {
//	leds = (int ) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
//}
//void add() {
//	for (int i = 0; i < 12; i++) {
//
//	}
//}
//void repeat() {
//	if (instCnt + 1 == instMax)
//		return;
//
//	for (int i = 0; i < 5; i++)
//		instuctions[instCnt + 1]();
//}
//
//instructions_t instuctions[] = { load, repeat, add };

