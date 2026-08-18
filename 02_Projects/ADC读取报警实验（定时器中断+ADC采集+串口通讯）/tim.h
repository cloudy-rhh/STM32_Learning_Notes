#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx_hal.h"

void tim2_init(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
	
#endif
