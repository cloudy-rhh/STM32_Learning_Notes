/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2020-08-20
 * @brief    新建工程实验-HAL库版本 实验
 * @license  Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ******************************************************************************
 * @attention
 * 
 * 实验平台:正点原子 STM32F103 开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "../Drivers/BSP/LCD/lcd.h"
#include "exti.h"
#include <stdio.h>
#include <string.h>

extern uint8_t g_usart_rx_buf[USART_REC_LEN]; 
extern uint16_t g_usart_rx_sta; 

uint16_t state = 0;
uint16_t key_state = 0;
uint16_t key_puts = 0;
uint16_t change_state = 0;

uint16_t CNBR_num = 0;
uint16_t VNBR_num = 0;
uint16_t ALL_num = 8;

uint32_t current_time = 0;
uint32_t key_time = 0;

double CNBR_rate = 3.50;
double VNBR_rate = 2.00;

char cmd_type[5];
char cmd_letter;
int cmd_num;
char time_str[13];

char type[5];
char letter;
int num;
char time[13];

struct park_space{
		uint8_t used;
		char info[30];
};

struct park_space parking_line[8];

void park_in(char* pk);

int find_car(char* pk);

int park_hour(char* park_in_time, char* park_out_time);

void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
		uint8_t CNBR_D[20];
		uint8_t VNBR_D[20];
		uint8_t IDLE_D[20];
	
		uint8_t CNBR_P[20];
		uint8_t VNBR_P[20];
	
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    led_init();                         /* LED初始化 */
		exti_init();
		usart_init(9600);
		lcd_init();
		g_point_color = WHITE;
		g_back_color = BLACK;
		lcd_clear(BLACK);
	
		HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t*)g_usart_rx_buf, 1);
	
    while(1)
    { 
				current_time = HAL_GetTick();
				
				if(key_puts == 1)
				{
						if(current_time - key_time >= 10)
						{
								if(HAL_GPIO_ReadPin(KEY_UP_PORT, KEY_UP_PIN) == GPIO_PIN_SET)
								{
										state = !state;
										change_state = 1;
								}
								
								else if(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == GPIO_PIN_RESET)
								{
										if(state == 1)
										{
												CNBR_rate += 0.50;
												VNBR_rate += 0.50;
										}
								}
								
								else if(HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == GPIO_PIN_RESET)
								{
										key_state = !key_state;
								}
								
								else if(HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == GPIO_PIN_RESET)
								{
										if(state == 1)
										{
												CNBR_rate -= 0.50;
												VNBR_rate -= 0.50;
										}
								}
								
								key_puts = 0;
						}
				}
				
				uint16_t IDLE_num = ALL_num - CNBR_num - VNBR_num;
				
				if(g_usart_rx_sta & 0x8000)
				{
						char pk[30];
						strcpy(pk, (char*)g_usart_rx_buf);
					
						if(sscanf(pk, "%4s:%c%3d:%12s", cmd_type, &cmd_letter, &cmd_num, time_str) == 4)
						{
								if(find_car(pk) == 0)
								{
										if(IDLE_num > 0)
										{
												if(strcmp(cmd_type , "CNBR") == 0)
												{
														CNBR_num++;
														printf("串口接收车辆入停车场信息  %s\r\n", pk);
														park_in(pk);
												}
								
												else if(strcmp(cmd_type, "VNBR") == 0)
												{
														VNBR_num++;
														printf("串口接收车辆入停车场信息  %s\r\n", pk);
														park_in(pk);
												}
								
										}
						
										else
										{
												printf("Error\r\n");
										}
								}
								
								else
								{
										char fee_info[20];
										int hour = 0;
										double fee = 0;
									
										sscanf(parking_line[find_car(pk) - 1].info, "%4s:%c%3d:%12s", type, &letter, &num, time);
									
										if(strcmp(cmd_type , "CNBR") == 0)
										{
												CNBR_num--;
												printf("串口接收车辆出停车场信息  %s\r\n", pk);
												hour = park_hour(time, time_str);
												fee = hour * CNBR_rate;
										}
								
										else if(strcmp(cmd_type, "VNBR") == 0)
										{
												VNBR_num--;
												printf("串口接收车辆出停车场信息  %s\r\n", pk);
												hour = park_hour(time, time_str);
												fee = hour * VNBR_rate;
										}
										
										sprintf(fee_info, "%s:%c%3d:%d:%.2f", cmd_type, cmd_letter, cmd_num, hour, fee);
										
										printf("串口输出计费信息: %s\r\n" ,fee_info);
										
										parking_line[find_car(pk) - 1].used = 0;
										memset(parking_line[find_car(pk) - 1].info, 0, sizeof(parking_line[find_car(pk) - 1].info));
								}
						}
						
						else
						{
								printf("Error\r\n");
						}
						
						g_usart_rx_sta = 0;
				}
				
				if(state == 0)
				{
						if(change_state == 1)
						{
								lcd_fill(40, 40, 279, 143, BLACK);
								change_state = 0;
						}
						sprintf((char*)CNBR_D, "CNBR: %d", CNBR_num);
						sprintf((char*)VNBR_D, "VNBR: %d", VNBR_num);
						sprintf((char*)IDLE_D, "IDLE: %d", IDLE_num);
						lcd_show_string(40, 40, 240, 24, 24, "Data", WHITE);
						lcd_show_string(40, 80, 240, 24, 24, (char*)CNBR_D, WHITE);
						lcd_show_string(40, 120, 240, 24, 24, (char*)VNBR_D, WHITE);
						lcd_show_string(40, 160, 240, 24, 24, (char*)IDLE_D, WHITE);
				}
				
				else
				{
						if(change_state == 1)
						{
								lcd_fill(40, 40, 279, 183, BLACK);
								change_state = 0;
						}
						sprintf((char*)CNBR_P, "CNBR: %.2f", CNBR_rate);
						sprintf((char*)VNBR_P, "VNBR: %.2f", VNBR_rate);
						lcd_show_string(40, 40, 240, 24, 24, "Para", WHITE);
						lcd_show_string(40, 80, 240, 24, 24, (char*)CNBR_P, WHITE);
						lcd_show_string(40, 120, 240, 24, 24, (char*)VNBR_P, WHITE);
				}
			
				if(IDLE_num > 0)
				{
						HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
				}
				
				else
				{
						HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
				}
				
				if(key_state == 0)
				{
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
				}
				
				else
				{
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
				}
				
				HAL_Delay(10);
    }
}

/**
 * @brief       初始化LED相关IO口, 并使能时钟
 * @param       无
 * @retval      无
 */
void led_init(void)
{
    GPIO_InitTypeDef gpio_initstruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();                          /* IO口PB时钟使能 */
    __HAL_RCC_GPIOE_CLK_ENABLE();                          /* IO口PE时钟使能 */

    gpio_initstruct.Pin = GPIO_PIN_5;                      /* LED0引脚 */
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_initstruct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOB, &gpio_initstruct);                /* 初始化LED0引脚 */

    gpio_initstruct.Pin = GPIO_PIN_5;                      /* LED1引脚 */
    HAL_GPIO_Init(GPIOE, &gpio_initstruct);                /* 初始化LED1引脚 */
}

void park_in(char* pk)
{
		for(int i = 0; i < 8; i++)
		{
				if(parking_line[i].used == 0)
				{
						strcpy(parking_line[i].info, pk);
						parking_line[i].used = 1;
						break;
				}
		}
}

int find_car(char* pk)
{
		for(int i = 0; i < 8; i++)
		{
				if(parking_line[i].used == 1 && strncmp(parking_line[i].info, pk, 9) == 0)
				{
						return (i + 1);
				}
		}
		return 0;
}

int park_hour(char* park_in_time, char* park_out_time)
{
		int h_in, m_in, h_out, m_out;
   
    sscanf(park_in_time + 8, "%2d%2d", &h_in, &m_in);
    sscanf(park_out_time + 8, "%2d%2d", &h_out, &m_out);
    
    int in_min = h_in * 60 + m_in;
    int out_min = h_out * 60 + m_out;
    int diff = out_min - in_min;
    
    if (diff < 0) {
        diff += 1440; 
    }
    
    int hours = diff / 60;
		
    if (diff % 60 > 0) {
        hours++;
    }
		
    return hours;
}
