#include "tim.h"

TIM_HandleTypeDef g_tim2_handle;

void tim2_pwm_init(void)
{
		GPIO_InitTypeDef gpio_initstruct;
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM2_CLK_ENABLE();
		
		gpio_initstruct.Pin = GPIO_PIN_0;
		gpio_initstruct.Mode = GPIO_MODE_AF_PP;
		gpio_initstruct.Pull = GPIO_PULLUP;
		gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpio_initstruct);
	
		g_tim2_handle.Instance = TIM2;
		g_tim2_handle.Init.Prescaler = 71;
		g_tim2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
		g_tim2_handle.Init.Period = 999;
		g_tim2_handle.Init.AutoReloadPreload = ENABLE;
		HAL_TIM_PWM_Init(&g_tim2_handle);
	
		TIM_OC_InitTypeDef sConfig;
		sConfig.OCMode = TIM_OCMODE_PWM1;
		sConfig.Pulse = 500;
		sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfig.OCFastMode = TIM_OCFAST_DISABLE;
		HAL_TIM_PWM_ConfigChannel(&g_tim2_handle, &sConfig, TIM_CHANNEL_1);
		
		HAL_TIM_PWM_Start(&g_tim2_handle, TIM_CHANNEL_1);
		
		HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		HAL_TIM_Base_Start_IT(&g_tim2_handle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance == TIM2)
		{
				static uint32_t duty = 0;
				static uint16_t cMode = 1;
				static uint16_t tick = 0;
			
				switch(Mode)
				{
						case 0:
								HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
								__HAL_TIM_SET_COMPARE(&g_tim2_handle, TIM_CHANNEL_1, 500);
								break;
						
						case 1:
								if(cMode)
								{
										duty += 1;
										if(duty >= 999)
										{
												duty = 999;
												cMode = 0;
										}
								}
								else
								{
										duty -= 1;
										if(duty <= 0)
										{
												duty = 0;
												cMode = 1;
										}
								}
								
								__HAL_TIM_SET_COMPARE(&g_tim2_handle, TIM_CHANNEL_1, duty);
								
								break;
								
						case 2:
								tick++;
						
								if(tick >= 500)
								{
										tick = 0;
										HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
								}
								
								break;
				}
		}
}
