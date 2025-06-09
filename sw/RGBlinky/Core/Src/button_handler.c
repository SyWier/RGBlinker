#include "button_handler.h"

#define BTN_DEBOUNCE_MS       20      // ignore contact bounce
#define BTN_INTERCLICK_MS    400      // max gap before 2nd click
#define BTN_LONG_MS          800      // press ≥ 0.8 s
#define BTN_VLONG_MS        2000      // press ≥ 2 s

static inline uint8_t btn_level(Button *b)
{
    return HAL_GPIO_ReadPin(b->port, b->pin);   // assumes active-low push-button
}

void Button_Tick(Button *b)
{
    switch (b->state)
    {
    case BTN_IDLE:
        if (btn_level(b)) {                    // pressed?
            b->state   = BTN_DEBOUNCE_PRESS;
            b->t_stamp = 0;
        }
        break;

    case BTN_DEBOUNCE_PRESS:
        if (++b->t_stamp >= BTN_DEBOUNCE_MS) {
            if (btn_level(b)) {                // still pressed → valid
                b->state   = BTN_PRESSED;
                b->t_stamp = 0;
            } else {                            // bounce
                b->state = BTN_IDLE;
            }
        }
        break;

    case BTN_PRESSED:
        b->t_stamp++;
        if (!btn_level(b)) {                     // released
            b->state   = BTN_DEBOUNCE_RELEASE;
            b->t_stamp = 0;
        } else {
            if (b->t_stamp == BTN_LONG_MS)
                b->on_long();
            else if (b->t_stamp == BTN_VLONG_MS)
                b->on_vlong();
        }
        break;

    case BTN_DEBOUNCE_RELEASE:
        if (++b->t_stamp >= BTN_DEBOUNCE_MS) {
            if (!btn_level(b)) {                 // stable high = valid release
                if (b->click_count == 0) {
                    b->click_count = 1;
                    b->state       = BTN_WAIT_SECOND;
                    b->t_stamp     = 0;
                } else {                        // second release inside debounce
                    b->click_count = 0;
                    b->on_double();
                    b->state = BTN_IDLE;
                }
            } else {                            // false release
                b->state = BTN_PRESSED;
            }
        }
        break;

    case BTN_WAIT_SECOND:
        b->t_stamp++;
        if (btn_level(b)) {                    // second press arrived
            b->state       = BTN_DEBOUNCE_PRESS;
            b->t_stamp     = 0;
            b->click_count = 2;
        } else if (b->t_stamp > BTN_INTERCLICK_MS) {
            // gap elapsed → single click confirmed
            b->click_count = 0;
            b->on_single();
            b->state = BTN_IDLE;
        }
        break;
    }
}

void on_single_click(void)  { printf("single\r\n"); }
void on_double_click(void)  { printf("double!\r\n"); }
void on_long_press(void)    { printf("long\r\n"); }
void on_vlong_press(void)   { printf("very long!\r\n"); }

Button userBtn = {
    .port = GPIOB,
    .pin  = GPIO_PIN_6,
    .on_single = on_single_click,
    .on_double = on_double_click,
    .on_long   = on_long_press,
    .on_vlong  = on_vlong_press
};
