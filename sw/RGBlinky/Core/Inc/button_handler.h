/*
 * button_handler.h
 *
 *  Created on: Jun 9, 2025
 *      Author: Daniel
 */

#ifndef INC_BUTTON_HANDLER_H_
#define INC_BUTTON_HANDLER_H_

#include "stm32c0xx_hal.h"

typedef enum {
    BTN_IDLE,
    BTN_DEBOUNCE_PRESS,
    BTN_PRESSED,
    BTN_DEBOUNCE_RELEASE,
    BTN_WAIT_SECOND
} btn_state_t;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;

    btn_state_t   state;
    uint32_t      t_stamp;
    uint8_t       click_count;

    void (*on_single)(void);
    void (*on_double)(void);
    void (*on_long)(void);
    void (*on_vlong)(void);
} Button;

void Button_Tick(Button *b);

#endif /* INC_BUTTON_HANDLER_H_ */
