#ifndef __io
#define __io 
#include "sys.h"


extern uint8_t  PWM_LED;
typedef struct
{
    u8  LED_Adjust_Flag;
    u8  LED_ID;
    u8  LED_State;
    u16 LED1_PWM;
    u16 LED2_PWM;
    u16 LED3_PWM;
    u16 LED4_PWM;
    u8  LED_SaveFlag;
    int LED_Offset;
}Adjust_LED;

#define LED PDout(2)
#define LED_ON  0
#define LED_OFF 1
void ST_LED_Init(void);

//左右手电机
#define LeftHand_Pulse    PBout(3)
#define LeftHand_Dir      PBout(8)
#define RightHand_Pulse   PBout(7)
#define RightHand_Dir     PBout(5)

//左右手气爪
#define LeftHand_Air      PBout(4)
#define RightHand_Air     PBout(6)
void ST_Hands_IO_Init(void);



//气动总阀门
#define MainValve         PCout(12)
#define Beep1             PCout(11)
#define Beep              PCout(10)
void ST_MainValve_IO_Init(void);

//板载按键  Key1-6 对应板子上S3-S8
#define Key1			  PBin(10)
#define Key2			  PBin(11)
#define Key3			  PBin(12)
#define Key4			  PBin(13)
#define Key5			  PBin(14)
#define Key6			  PBin(15)
//开始信号按键
#define Key_Start         PAin(15)
void ST_Key_IO_Init(void);
#define down 1

#endif



