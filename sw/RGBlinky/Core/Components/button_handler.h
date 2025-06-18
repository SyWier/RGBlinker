/*
 * button_handler.h
 *
 *  Created on: Jun 9, 2025
 *      Author: Daniel
 */

#ifndef INC_BUTTON_HANDLER_H_
#define INC_BUTTON_HANDLER_H_

#include "gpio.h"
#include "stdbool.h"

#define BTN_DEBOUNCE_MS	50		// Ignore contact bounce
#define BTN_DOUBLE_MS	200     // Max gap before 2nd click
#define BTN_LONG_MS		2000    // Press ≥ 2.0 s
#define BTN_VLONG_MS	5000    // Press ≥ 5.0 s

typedef enum {
	BTN_BOOT,
    BTN_IDLE,
    BTN_PRESSED,
	BTN_RELEASED,
	BTN_DOUBLE
} btn_state_t;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;

    btn_state_t   state;
    uint32_t      t_stamp;

    bool is_initialized;

    void (*action)(void);
} Button;

extern Button userBtn;

void Button_Init(Button *b);
void Button_Tick(Button *b);

#endif /* INC_BUTTON_HANDLER_H_ */
