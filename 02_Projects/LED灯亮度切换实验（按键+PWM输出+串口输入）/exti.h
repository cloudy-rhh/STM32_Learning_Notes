#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f1xx_hal.h"
extern uint16_t brightness_level;
extern TIM_HandleTypeDef g_tim2_handle;

void exti_init(void);

uint16_t pulse_dep(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
