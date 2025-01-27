#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	PWM_Init();
	
	
}
void Motor_set_speed(int8_t speed)
{
	if(speed >=0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4 );
		GPIO_ResetBits(GPIOA, GPIO_Pin_5 );
		SetCompare3(speed);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_5 );
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 );
		SetCompare3(speed);
	}
}
