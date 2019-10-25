#ifndef __beep_H
#define __beep_H

#include "sys.h"

//LED0Òý½Å    GPIOF-9
#define Beep_GPIO_CLK  RCC_AHB1Periph_GPIOF
#define Beep_GPIO_PORT GPIOF
#define Beep_GPIO_Pin  GPIO_Pin_8

#define beep PFout(8)	// DS0

void beep_Init(void);//³õÊ¼»¯	

#endif

