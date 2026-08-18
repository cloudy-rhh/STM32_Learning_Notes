#ifndef __ADC_H
#define __ADC_H

#include "stm32f1xx_hal.h"
extern uint16_t adc_complete;
extern uint32_t adc_value;

void adc_init(void);

void adc_read(void);

#endif
