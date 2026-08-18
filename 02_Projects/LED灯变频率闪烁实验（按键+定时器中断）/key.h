#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"
extern uint8_t g_speed_mode;

#define KEY0_PORT  GPIOE
#define KEY0_PIN   GPIO_PIN_4

void key0_init(void);

#endif
