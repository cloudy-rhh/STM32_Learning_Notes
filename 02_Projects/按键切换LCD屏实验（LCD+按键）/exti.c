#include "exti.h"

void exti_init(void)
{
		GPIO_InitTypeDef gpio_initstruct;
		__HAL_RCC_GPIOE_CLK_ENABLE();
		__HAL_RCC_SYSCFG_CLK_ENABLE();
	
		gpio_initstruct.Pin = KEY_PIN;
		gpio_initstruct.Mode = GPIO_MODE_IT_FALLING;
		gpio_initstruct.Pull = GPIO_PULLUP;
		gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(KEY_PORT, &gpio_initstruct);
	
		HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY_PIN) 
    {
				key_time = HAL_GetTick();
				key_puts = 1;
    }
}
