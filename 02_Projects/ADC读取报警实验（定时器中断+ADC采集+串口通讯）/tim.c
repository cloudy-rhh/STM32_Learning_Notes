#include "tim.h"
#include "adc.h"

TIM_HandleTypeDef g_tim2_handle;

void tim2_init(void)
{
		__HAL_RCC_TIM2_CLK_ENABLE();
		
		g_tim2_handle.Instance = TIM2;
		g_tim2_handle.Init.Prescaler = 7199;
		g_tim2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
		g_tim2_handle.Init.Period = 4999;
		g_tim2_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		
		HAL_TIM_Base_Init(&g_tim2_handle);
		HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		HAL_TIM_Base_Start_IT(&g_tim2_handle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance == TIM2)
		{
				adc_read();
		}
}
