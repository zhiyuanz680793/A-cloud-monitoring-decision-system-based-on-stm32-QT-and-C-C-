#include "stm32f10x.h"                  // Device header
extern uint16_t Compare1;
void PWM_Init(void)
{
	//Initialize TIME2 common Timer
	//RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//引脚重映射
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); ///AFIO用作引脚重映射
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//引脚重映射配置，部分重映射1或者全部重映射均可
	//见表43AFIO的TIM2引脚重映射
	//关闭调试端口的使用,因为PA15默认复用为调试端口JTDI
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	
	
	//初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//改为复用推挽输出模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//设置时基单元时钟为内部时钟 
	TIM_InternalClockConfig(TIM2);
	
	//初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1 ;//指定时钟分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStruct.TIM_Period=100-1;//ARR自动重装器的值
	TIM_TimeBaseInitStruct.TIM_Prescaler= 36-1;//PSC预分频器的值 ，主频=CK_PSC/[(PSC+1)*(ARR+1)].  
	//设频率为1khz，分辨率为1%，占空比为50%
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//重复计数器
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	//initialize 输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);//给定义的结构体赋初值
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1 ;//设置输出比较的模式
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High ;//set 输出比较的极性
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable ;//设置输出势能
	TIM_OCInitStruct.TIM_Pulse=0;     //设置CCR
	
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
		
	TIM_Cmd(TIM2,ENABLE);
	
}
void SetCompare3(uint16_t Compare1)
{
	TIM_SetCompare3(TIM2, Compare1);
}
