#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//配置ADCCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//配置GPIO0
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//选择规则组输入通道
	//单AD模式取消注释
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef  ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADC1,&ADC_InitStruct);
	//Ifneeded 配置中断看门狗在这儿
	
	ADC_Cmd(ADC1,ENABLE);
	
	//校准
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//等待复位校准完成
	ADC_StartCalibration(ADC1);//开始校准
	while (ADC_GetCalibrationStatus(ADC1)==SET);//等待校准完成

}
uint16_t ADC_Get_Value(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_55Cycles5);//指定通道，返回指定通道的结果
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);//获取EOC转换结束标志位状态 0：未完成，1：完成,//未完成则继续等待
	return ADC_GetConversionValue(ADC1);//获取转换植,读取ADC->DR寄存器,会自动清除EOC标志位，不需要手动清除了
	
}
