#include "ST_ADC.h"
void ST_ADC_Init(u8 Adc_CH)
{
GPIO_InitTypeDef       GPIO_InitStructure;
ADC_CommonInitTypeDef  ADC_CommonInitStructure;
ADC_InitTypeDef        ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��	
	if(Adc_CH<=7)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
		switch (Adc_CH)
        {
        	case 0:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 ͨ��0  ��ͬ
        		break;
        	case 1:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        		break;
			case 2:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        		break;
			case 3:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        		break;
			case 4:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        		break;
			case 5:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        		break;
			case 6:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        		break;
			case 7:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        		break;
        	default:
        		break;
        }
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
	}
	else if((Adc_CH==8)||(Adc_CH==9))
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
		if(Adc_CH==8)  
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //ͨ��8
		else           
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;   //ͨ��9
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� 
	}
	else if(Adc_CH>9)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
		switch (Adc_CH)
        {
        	case 10:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PC0 ͨ��10  ��ͬ
        		break;
        	case 11:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        		break;
			case 12:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        		break;
			case 13:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        		break;
			case 14:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        		break;
			case 15:GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        		break;
        	default:
        		break;
        }
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
		GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
	}
	
	
	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����
	
	ADC_DeInit();
	
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��

	ADC_DeInit();
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	ADC_Cmd(ADC1, ENABLE);//����ADת����
}
u16 ST_GetAdcValue(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

