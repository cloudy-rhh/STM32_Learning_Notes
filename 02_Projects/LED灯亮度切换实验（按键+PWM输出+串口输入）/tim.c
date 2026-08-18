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
		g_tim2_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		HAL_TIM_PWM_Init(&g_tim2_handle);
	
		TIM_OC_InitTypeDef oc_config;
		oc_config.OCMode = TIM_OCMODE_PWM1;
		oc_config.Pulse = 250;
		oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
		oc_config.OCFastMode = TIM_OCFAST_DISABLE;
		HAL_TIM_PWM_ConfigChannel(&g_tim2_handle, &oc_config, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&g_tim2_handle, TIM_CHANNEL_1);
}
