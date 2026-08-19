#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx_hal.h"
extern uint16_t Mode;

void tim2_pwm_init(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
	
#endif
