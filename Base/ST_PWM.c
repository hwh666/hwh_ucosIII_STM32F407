#include "ST_PWM.h"
#define ACCURACY 1000//满占空比为5000
#define INIT_DUTY 0
//定时器1的pwm带互补输出的 
void ST_TIM1_PWM_Init(u16 freq,u8 mode)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure; 
	uint16_t PrescalerValue = 0;
	u32 freq_set = ACCURACY*freq;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM1);



	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ( ( SystemCoreClock ) / freq_set ) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = ACCURACY;									
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	if(mode==PWM_Polarity_High)
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	else 
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;       //在空闲时输出     高  
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;    //在空闲时互补输出 低
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */

	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	
	TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Disable; 
	TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Disable; 
	TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF; 
	TIM_BDTRInitStructure.TIM_DeadTime=0; 
	TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable; 
	TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High; 
	TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable; 
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure); 
	TIM_CtrlPWMOutputs(TIM1,ENABLE); 
	TIM_Cmd(TIM1, ENABLE);
	
}
void ST_TIM2_PWM_Init(u16 freq,u8 mode)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t PrescalerValue = 0;
	u32 freq_set = ACCURACY*freq;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);


	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ( ( SystemCoreClock /2 ) / freq_set ) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = ACCURACY;									
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	if(mode==PWM_Polarity_High)
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	else 
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
}
void ST_TIM3_PWM_Init(u16 freq,u8 mode)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t PrescalerValue = 0;
	u32 freq_set = ACCURACY*freq;
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_OCStructInit(&TIM_OCInitStructure);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	  
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	
	
/* Compute the prescaler value */
  PrescalerValue = (uint16_t) ( ( SystemCoreClock /2 ) / freq_set ) - 1;
 /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = ACCURACY;									
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   if(mode==PWM_Polarity_High)
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   else 
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM3, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
	
}
//void ST_TIM4_PWM_Init(u16 freq,u8 mode)
//{
//	//...copy and paste here
//}
//void ST_TIM5_PWM_Init(u16 freq,u8 mode)
//{
//	//...copy and paste here
//}

void ST_PWM2_Set(u16 CH1_Value,u16 CH2_Value,u16 CH3_Value,u16 CH4_Value)
{
	TIM2->CCR1 = CH1_Value;
	TIM2->CCR2 = CH2_Value;
	TIM2->CCR3 = CH3_Value;
	TIM2->CCR4 = CH4_Value;
}
void ST_PWM3_Set(u16 CH1_Value,u16 CH2_Value,u16 CH3_Value,u16 CH4_Value)
{
	TIM3->CCR1 = CH1_Value;
	TIM3->CCR2 = CH2_Value;
	TIM3->CCR3 = CH3_Value;
	TIM3->CCR4 = CH4_Value;
}

//void ST_PWM4_Set(u16 CH1_Value,u16 CH2_Value,u16 CH3_Value,u16 CH4_Value)
//{
//	TIM4->CCR1 = CH1_Value;
//	TIM4->CCR2 = CH2_Value;
//	TIM4->CCR3 = CH3_Value;
//	TIM4->CCR4 = CH4_Value;
//}
//void ST_PWM5_Set(u16 CH1_Value,u16 CH2_Value,u16 CH3_Value,u16 CH4_Value)
//{
//	TIM5->CCR1 = CH1_Value;
//	TIM5->CCR2 = CH2_Value;
//	TIM5->CCR3 = CH3_Value;
//	TIM5->CCR4 = CH4_Value;
//}
void ST_SetMotor(u16 motor1,u16 motor2,u16 motor3,u16 motor4)
{
	TIM3->CCR1 = motor1;
	TIM3->CCR2 = motor2;
	TIM3->CCR4 = motor3;
	TIM3->CCR3 = motor4;  //3和4画图为了布线方便调换了
}

