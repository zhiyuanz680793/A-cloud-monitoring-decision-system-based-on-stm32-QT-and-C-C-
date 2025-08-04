#ifndef __SERIAL_H
#define __SERIAL_H

void Serial_SendByte(uint8_t Byte);//发送原始数据
void Serial_Init(void);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char* string);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);//使用文本模式显示原始数字
uint8_t Serial_GetRxFlag(void);
uint16_t Serial_GetRxData(void);

#endif
