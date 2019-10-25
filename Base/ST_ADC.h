#ifndef __ADC
#define __ADC
#include "sys.h"
enum 
{
	AD_CH0 =0,
	AD_CH1 ,
	AD_CH2 ,
	AD_CH3 ,
	AD_CH4 ,
	AD_CH5 ,
	AD_CH6 ,
	AD_CH7 ,
	AD_CH8 ,
	AD_CH9 , 
	AD_CH10 ,
	AD_CH11 ,
	AD_CH12 ,
	AD_CH13 ,
	AD_CH14 ,
	AD_CH15 
};


#define BatteryVol_ch  AD_CH4
void ST_ADC_Init(u8 Adc_CH);
u16 ST_GetAdcValue(u8 ch);
#endif


