#include "exti.h"
#include <stdio.h>

void exti_init(void)
{
		GPIO_InitTypeDef gpio_initstruct;
		__HAL_RCC_GPIOE_CLK_ENABLE();
		__HAL_RCC_SYSCFG_CLK_ENABLE();
		
		gpio_initstruct.Pin = GPIO_PIN_2;
		gpio_initstruct.Mode = GPIO_MODE_IT_FALLING;
		gpio_initstruct.Pull = GPIO_PULLUP;
		gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOE, &gpio_initstruct);
	
		HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if(GPIO_Pin == GPIO_PIN_2)
		{
				HAL_Delay(20);
			
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_Pin) == GPIO_PIN_RESET)
				{
						if(Mode < 2)
						{
								Mode++;
						}
						else
						{
								Mode = 0;
						}
						printf("Mode: %d\r\n", Mode);
				}
		}
}
