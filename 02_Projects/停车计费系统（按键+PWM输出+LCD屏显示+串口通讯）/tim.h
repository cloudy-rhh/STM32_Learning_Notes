#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx_hal.h"

extern uint16_t key_state;

void tim2_pwm_init(void);

#endif
