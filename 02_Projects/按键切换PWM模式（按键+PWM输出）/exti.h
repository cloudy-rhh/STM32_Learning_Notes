#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f1xx_hal.h"
extern uint16_t Mode;

void exti_init(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
