#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f1xx_hal.h"

#define KEY_PORT  GPIOE
#define KEY_PIN  GPIO_PIN_4
extern uint16_t state;
extern uint16_t key_puts;
extern uint32_t key_time;

void exti_init(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
