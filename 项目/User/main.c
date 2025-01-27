#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Control.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "servo.h"
#include "Key.h"
#include "Motor.h"
#include "AD.h"

uint8_t key;
uint16_t Compare1;
float key_number;
float angle;//调试舵机角度，没有使用到

int8_t speed;//风扇转速

float Voltage;//热敏电阻决定的电压值
uint16_t AD0,AD7;//ADC原始读入
uint16_t ADC_t;//ADC临时数据读入
//uint8_t light;//光敏电阻
int main(void)
{


	OLED_Init();
	//Key_Init();
	//Servo_Init();
	//Servo_Set_Angle(0);
	//OLED_ShowChar(1,2,'A');
	//OLED_ShowString(2,2,"Zhiyuan");
	//OLED_ShowSignedNum(1,6,12345,4);
	//SetCompare2(1500);
	Motor_Init();
	Motor_set_speed(0);
	OLED_ShowString(1,1,"Speed:");
	Key_Init();
	OLED_ShowString(2,1,"AD:");//热敏电阻ADC
	OLED_ShowString(3,1,"Voltage:");//电压
	OLED_ShowString(4,8,":");//电压小数
	AD_Init();
	//OLED_ShowString(2,1,"AD7:");//螺丝调节电位器
	while (1)
	{
		AD0 = ADC_Get_Value(ADC_Channel_0); //热敏电阻
		//OLED_ShowSignedNum(1,6,AD0,4);//抖动可以用上下阈值法
		ADC_t =AD0;		
		AD7 = ADC_Get_Value(ADC_Channel_7);//电位器
		OLED_ShowSignedNum(2,6,ADC_t,4);

		Voltage=(float)ADC_t/4095*3.3;
		OLED_ShowSignedNum(3,10,Voltage,1);
		OLED_ShowSignedNum(4,10,(uint16_t)(Voltage*100)%100,2);
		key = Key_GetNum();
		/*按键切换风扇转速*/
		/*if(key==1)
		{
			speed+=10;
			if(speed>10)
			{
				speed = -10;
			}
			Motor_set_speed(speed);
		}*/
		
		/*纯STM32实现控制*/
		/*根据热敏电阻/ADC调节风扇转速*/
		/*电压0~3.3V,ADC范围0~4095*/
		/*风扇转速-设置0~50*/
		
		//ADC映射转速,并设置
		speed=(int8_t)((float)ADC_t/4095*50);
		Motor_set_speed(speed);
		//显示转速
		OLED_ShowSignedNum(1,7,speed,4);
		Delay_ms(100);
	}
}
