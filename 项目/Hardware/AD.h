#ifndef __AD_H
#define __AD_H

extern uint16_t AD_Value[4];
//uint16_t ADC_Get_Value(uint8_t ADC_Channel);//不用DMA
void ADC_Get_Value(void);//调用DMA
void AD_Init(void);
#endif
