#include "stm32f10x.h"                  // Device header
//#include "LED.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_2 );
	
}
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}
