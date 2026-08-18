#include "adc.h"

ADC_HandleTypeDef g_adc_handle;

void adc_init(void)
{
		GPIO_InitTypeDef gpio_initstruct;
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_ADC1_CLK_ENABLE();
	
		gpio_initstruct.Pin = GPIO_PIN_1;
		gpio_initstruct.Mode = GPIO_MODE_ANALOG;
		gpio_initstruct.Pull = GPIO_NOPULL;
		gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpio_initstruct);
	
		g_adc_handle.Instance = ADC1;
		g_adc_handle.Init.ScanConvMode = DISABLE;
		g_adc_handle.Init.ContinuousConvMode = DISABLE;
		g_adc_handle.Init.DiscontinuousConvMode = DISABLE;
		g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		g_adc_handle.Init.NbrOfConversion = 1;
		HAL_ADC_Init(&g_adc_handle);
	
		ADC_ChannelConfTypeDef sConfig;
		sConfig.Channel = ADC_CHANNEL_1;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
		HAL_ADC_ConfigChannel(&g_adc_handle, &sConfig);
}

void adc_read(void)
{
		HAL_ADC_Start(&g_adc_handle);
	
		if(HAL_ADC_PollForConversion(&g_adc_handle, 100) == HAL_OK)
		{
				adc_value = HAL_ADC_GetValue(&g_adc_handle);
		}
		
		HAL_ADC_Stop(&g_adc_handle);
		
		adc_complete = 1;
}
