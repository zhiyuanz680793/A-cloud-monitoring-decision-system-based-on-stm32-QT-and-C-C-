#include "stm32f10x.h"   
#include "PWM.h"
//#include "Key.h"

void Servo_Init()
{
	PWM_Init();
}

void Servo_Set_Angle(float angle)
{
	SetCompare3(angle/180*2000+500);
}
