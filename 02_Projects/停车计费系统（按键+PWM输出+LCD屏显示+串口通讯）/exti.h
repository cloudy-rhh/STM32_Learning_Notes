#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f1xx_hal.h"

#define KEY0_PORT  GPIOE
#define KEY0_PIN  GPIO_PIN_4

#define KEY1_PORT  GPIOE
#define KEY1_PIN  GPIO_PIN_3

#define KEY2_PORT  GPIOE
#define KEY2_PIN  GPIO_PIN_2

#define KEY_UP_PORT  GPIOA
#define KEY_UP_PIN  GPIO_PIN_0

extern uint16_t state;
extern uint16_t key_state;
extern uint16_t key_puts;

extern uint32_t key_time;

extern double CNBR_rate;
extern double VNBR_rate;

void exti_init(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
