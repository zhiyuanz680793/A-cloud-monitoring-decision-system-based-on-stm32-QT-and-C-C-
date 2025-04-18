#include "stm32f10x.h"                  // Device header
#include "Delay.h"
extern uint8_t Mode;
extern uint8_t key;
/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	
	
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB1和PB11引脚初始化为上拉输入
	/*AFIO选择中断引脚*/
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);//将外部中断的1和11号线映射到GPIOB，即选择PB11和PB1为外部中断引脚

	/*EXTI初始化*/	
	/* 配置 EXTI1 */
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	
	/* 配置 EXTI11 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_Init(&EXTI_InitStructure);
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2//USART优先级为1
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置1*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;;		//选择配置NVIC的EXTI15_10线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
	

}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		//定义变量，默认键码值为0
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 1;												//置键码为1
	}
	

	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}
/*
uint8_t Key_Change_Mode(uint8_t key)
{
	return (key+1)%2;	
}*/
/*
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET)
    {
        
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}*/
void EXTI15_10_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line11) == SET)		//判断是否是外部中断11号线触发的中断
	{
		Mode=(Mode+1)%2;
		EXTI_ClearITPendingBit(EXTI_Line11);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除											//否则中断将连续不断地触发，导致主程序卡死
	}
}

