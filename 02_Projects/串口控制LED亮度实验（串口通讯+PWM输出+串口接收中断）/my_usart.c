#include "my_usart.h"

uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_byte;
uint8_t rx_index = 0;
uint8_t rx_complete = 0;

UART_HandleTypeDef g_uart1_handle;

void my_usart_init(void)
{
		GPIO_InitTypeDef gpio_initstruct;
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();
	
		//TX位传输端
		gpio_initstruct.Pin = GPIO_PIN_9;
		gpio_initstruct.Mode = GPIO_MODE_AF_PP;
		gpio_initstruct.Pull = GPIO_PULLUP;
		gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpio_initstruct);
	
		//RX为接收端
		gpio_initstruct.Pin = GPIO_PIN_10;
		gpio_initstruct.Mode = GPIO_MODE_AF_INPUT;
		HAL_GPIO_Init(GPIOA, &gpio_initstruct);
	
		g_uart1_handle.Instance = USART1;
		g_uart1_handle.Init.BaudRate = 115200;
		g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;
		g_uart1_handle.Init.StopBits = UART_STOPBITS_1;
		g_uart1_handle.Init.Parity = UART_PARITY_NONE;
		g_uart1_handle.Init.Mode = UART_MODE_TX_RX;
		g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		g_uart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;
		HAL_UART_Init(&g_uart1_handle);
		
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
}

int fputc(int ch, FILE *f)
{
		HAL_UART_Transmit(&g_uart1_handle, (uint8_t*)&ch, 1, 0xFFFF);
		return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		if(huart->Instance == USART1)
		{
				if(rx_byte == '\n')
				{
						rx_buffer[rx_index] = '\0';
						rx_complete = 1;
				}
				else if(rx_index <= RX_BUFFER_SIZE - 1 && rx_byte != '\r')
				{
						rx_buffer[rx_index++] = rx_byte;
				}
				HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t*)&rx_byte, 1);
		}
}
