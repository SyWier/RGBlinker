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

typedef enum {
	BTN_BOOT,
    BTN_IDLE,
    BTN_SINGLE,
	BTN_RELEASED,
	BTN_DOUBLE,
	BTN_LONG,
	BTN_EXTRA,
} btn_state_t;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;

    btn_state_t   state;
    uint32_t      t_stamp;

    bool is_initialized;
} Button;

extern Button userBtn;

void Button_Init(Button *b);
void Button_Tick(Button *b);

#endif /* INC_BUTTON_HANDLER_H_ */
