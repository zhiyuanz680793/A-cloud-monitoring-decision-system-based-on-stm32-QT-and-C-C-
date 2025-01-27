#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void Control_Init(void)
{
	//clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin= GPIO_Pin_11 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//Initializing the bits to zeros
	GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_11);
}
uint8_t Get_Key_Number(void)
{
	uint8_t key =0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)//push key 1
	{
		Delay_ms(40);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		Delay_ms(40);
		key =1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)//push key 11
	{
		Delay_ms(40);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		Delay_ms(40);
		key =2;
	}
	return key;
}
void LED1_Turn(void)
{
	//uint8_t key;
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_0)==0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	}
}
void LED2_Turn(void)
{
	//uint8_t key;
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
}
