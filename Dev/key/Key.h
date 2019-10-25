#ifndef _Key_H_
#define _Key_H_

#include "sys.h"
#include "os.h"
#include "app_cfg.h"
#include "string.h"

void KEY_Init(void);
void Key_OSTaskCreate(void);

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP   	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0

#endif

