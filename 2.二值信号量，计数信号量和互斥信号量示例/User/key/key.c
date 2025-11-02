


#include "stm32f4xx.h"
#include "key.h"

/*
*--------------------------------------------------------------------------------------------------------
* Function:       key_init
* Description:    
* Input:          none
* Output:         none
* Return:         none
* Created by:     alvan 
* Created date:   2014-07-29
* Others:        	
*---------------------------------------------------------------------------------------------------------
*/
void  key_init (void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  /* 使能端口时钟                        */ 

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;	                             
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                 /* 配置引脚为上拉输入 	*/	 
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  /* 使能端口时钟                        */ 

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;	                             
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_PuPd_DOWN ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
}

uint8_t key_get(void)
{
    uint8_t key0 = (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET) ? 1 : 0;
    uint8_t key1 = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET) ? 2 : 0;
    
    return key0 + key1;  // 0:无按键, 1:按键0, 2:按键1, 3:同时按下
}
