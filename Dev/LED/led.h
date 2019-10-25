#ifndef __LED_H
#define __LED_H

#include "sys.h"
#include "os.h"
#include "app_cfg.h"


//LED0引脚    GPIOF-9
#define LED0_GPIO_CLK  RCC_AHB1Periph_GPIOF
#define LED0_GPIO_PORT GPIOF
#define LED0_GPIO_Pin  GPIO_Pin_9

//LED1引脚    GPIOF-10
#define LED1_GPIO_CLK  RCC_AHB1Periph_GPIOF
#define LED1_GPIO_PORT GPIOF
#define LED1_GPIO_Pin  GPIO_Pin_10

#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1
#define LED0_TOGGLE     {LED0_GPIO_PORT->ODR ^=LED0_GPIO_Pin;}	//LED0翻转电平
#define LED1_TOGGLE     {LED1_GPIO_PORT->ODR ^=LED1_GPIO_Pin;}	//LED1翻转电平

void LED_Init(void);//初始化	
void LED_OSTaskCreate(void);
#endif

