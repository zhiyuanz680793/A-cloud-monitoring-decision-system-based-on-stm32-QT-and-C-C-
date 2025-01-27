#include "stm32f10x.h"
// Device header
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}
void Buzzer_On(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
void Buzzer_Off(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
void Buzzer_Turn(void)
{
	uint8_t b ;
	b = GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12);
	if(b==0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
}
