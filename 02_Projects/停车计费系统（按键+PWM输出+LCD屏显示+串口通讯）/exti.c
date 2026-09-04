#include "exti.h"

void exti_init(void)
{
		GPIO_InitTypeDef gpio_initstruct;
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOE_CLK_ENABLE();
	
		__HAL_RCC_SYSCFG_CLK_ENABLE();
	
		//配置KEY_UP
		gpio_initstruct.Pin = KEY_UP_PIN;
		gpio_initstruct.Mode = GPIO_MODE_IT_RISING;
		gpio_initstruct.Pull = GPIO_PULLDOWN;
		gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(KEY_UP_PORT, &gpio_initstruct);
		
		//配置KEY0
		gpio_initstruct.Pin = KEY0_PIN;
		gpio_initstruct.Mode = GPIO_MODE_IT_FALLING;
		gpio_initstruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(KEY0_PORT, &gpio_initstruct);
		
		//配置KEY1
		gpio_initstruct.Pin = KEY1_PIN;
		HAL_GPIO_Init(KEY1_PORT, &gpio_initstruct);
	
		//配置KEY2
		gpio_initstruct.Pin = KEY2_PIN;
		HAL_GPIO_Init(KEY2_PORT, &gpio_initstruct);
		
		//设置KEY_UP中断优先级
		HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
		
		//设置KEY0中断优先级
		HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
		
		//设置KEY1中断优先级
		HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(EXTI3_IRQn);
		
		//设置KEY2中断优先级
		HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if(GPIO_Pin == KEY_UP_PIN || GPIO_Pin == KEY0_PIN || GPIO_Pin == KEY1_PIN || GPIO_Pin == KEY2_PIN)
		{
				key_puts = 1;
				key_time = HAL_GetTick();
		}
}
