#include "key.h"

void key0_init(void)
{
		GPIO_InitTypeDef gpio_initstruct;
		__HAL_RCC_GPIOE_CLK_ENABLE();
		
		gpio_initstruct.Pin = KEY0_PIN;
		gpio_initstruct.Mode = GPIO_MODE_INPUT;
		gpio_initstruct.Pull = GPIO_PULLUP;
		gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(KEY0_PORT, &gpio_initstruct);
}

