#include "exti.h"

void exti_init(void)
{
		GPIO_InitTypeDef gpio_initstruct;
	
		__HAL_RCC_GPIOE_CLK_ENABLE();
		__HAL_RCC_SYSCFG_CLK_ENABLE();
		
		gpio_initstruct.Pin = GPIO_PIN_4;
		gpio_initstruct.Mode = GPIO_MODE_IT_FALLING;
		gpio_initstruct.Pull = GPIO_PULLUP;
		gpio_initstruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOE, &gpio_initstruct);
		
		HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

uint16_t pulse_dep(void)
{
		if(brightness_level == 0)
		{
				return 250;
		}
		else if(brightness_level == 1)
		{
				return 500;
		}
		else
		{
				return 750;
		}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if(GPIO_Pin == GPIO_PIN_4)
		{
					if(brightness_level < 2)
				{
						brightness_level++;
				}
				else
				{
						brightness_level = 0;
				}
		
				uint16_t pulse = pulse_dep();
		
				__HAL_TIM_SET_COMPARE(&g_tim2_handle, TIM_CHANNEL_1, pulse);
		}
}
