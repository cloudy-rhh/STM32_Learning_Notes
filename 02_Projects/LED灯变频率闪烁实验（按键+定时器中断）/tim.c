#include "tim.h"
extern uint8_t g_speed_mode;

TIM_HandleTypeDef g_tim2_handle;

void tim2_init(void)
{
		__HAL_RCC_TIM2_CLK_ENABLE();
	
		g_tim2_handle.Instance = TIM2;
		g_tim2_handle.Init.Prescaler = 71;
		g_tim2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
		g_tim2_handle.Init.Period = 99999;
		g_tim2_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		HAL_TIM_Base_Init(&g_tim2_handle);
	
		HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		HAL_TIM_Base_Start_IT(&g_tim2_handle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		static uint8_t tick = 0;
		uint8_t count;
	
		if(htim->Instance == TIM2)
		{
				tick++;
			
				if(g_speed_mode == 0)
				{
						count = 5;
				}
				else
				{
						count = 2;
				}
				
				if(tick >= count)
				{
						tick = 0;
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
				}
		}
}
