/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"   

void  led_init (void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    /* 使能端口PORTC时钟                                  */  	 
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_6;               
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);                /* 配置引脚GPIOC.2为推挽输出,IO最大速度50M            */
  GPIO_SetBits(GPIOE , GPIO_Pin_5|GPIO_Pin_6);   
}