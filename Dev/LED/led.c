#include "led.h"

//---------------------------------������ƿ�
static  OS_TCB   LEDTaskTCB;                           //������ƿ�
//---------------------------------�����ջ
static  CPU_STK  LEDTaskStk[LED_TASK_STK_SIZE];       //�����ջ
//---------------------------------��������
void LED_Init(void);
static void LED_Task(void);
void LED_OSTaskCreate(void);


//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              LED��ʼ��
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LED0_GPIO_CLK, ENABLE);//ʹ��GPIOFʱ��

    //GPIOF9,F10��ʼ������
    GPIO_InitStructure.GPIO_Pin = LED0_GPIO_Pin | LED1_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);//��ʼ��
    
    GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_Pin | LED1_GPIO_Pin);//GPIOF9,F10���øߣ�����
}

//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              LED����
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------

static void LED_Task(void)
{
    OS_ERR  err; 
    for(;;)
    {
#if defined TimeMeasurement
    TimeMeasure_t.LED_Task_T[Old]=OS_TS_GET();
    Time_Cost.LED_Task_Time.Cycle_T=Get_Cycle_T(0);
    Time_Cost.LED_Task_Time.Cycle_T_Max=Time_Cost.LED_Task_Time.Cycle_T>Time_Cost.LED_Task_Time.Cycle_T_Max?
                                        Time_Cost.LED_Task_Time.Cycle_T:Time_Cost.LED_Task_Time.Cycle_T_Max;
#endif
/*----------------------contents of the task---------------------------------*/
	//task here
        LED0_TOGGLE
        OSTimeDlyHMSM ( 0, 0, 0, 50, OS_OPT_TIME_DLY, & err );  //ÿ500msɨ��һ��
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t.Key_Task_T[New]=OS_TS_GET();
    Time_Cost.LED_Task_Time.Task_T=(TimeMeasure_t.Key_Task_T[New]-TimeMeasure_t.LED_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost.LED_Task_Time.Task_T_Max=Time_Cost.LED_Task_Time.Task_T>Time_Cost.LED_Task_Time.Task_T_Max?
                                        Time_Cost.LED_Task_Time.Task_T:Time_Cost.LED_Task_Time.Task_T_Max;
#endif
    }
    
}

//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              ����LED����
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void LED_OSTaskCreate(void)
{
    OS_ERR  err;
    OSTaskCreate((OS_TCB     *) &LEDTaskTCB,                                //������ƿ��ַ
                 (CPU_CHAR   *)" LED Task Start",                          //��������
                 (OS_TASK_PTR ) LED_Task,                                   //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) LED_TASK_PRIO,                              //��������ȼ�
                 (CPU_STK    *)&LEDTaskStk[0],                              //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 0u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
}


