#ifndef __MY_USART_H
#define __MY_USART_H

#include "stm32f1xx_hal.h"
#include <stdio.h>
#define RX_BUFFER_SIZE 32

extern uint8_t rx_buffer[RX_BUFFER_SIZE];
extern uint8_t rx_byte;
extern uint8_t rx_index;
extern uint8_t rx_complete;

void my_usart_init(void);

int fputc(int ch, FILE *f);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif
