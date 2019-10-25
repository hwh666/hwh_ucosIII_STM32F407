#include "led.h"

//---------------------------------任务控制块
static  OS_TCB   LEDTaskTCB;                           //任务控制块
//---------------------------------任务堆栈
static  CPU_STK  LEDTaskStk[LED_TASK_STK_SIZE];       //任务堆栈
//---------------------------------函数声明
void LED_Init(void);
static void LED_Task(void);
void LED_OSTaskCreate(void);


//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              LED初始化
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LED0_GPIO_CLK, ENABLE);//使能GPIOF时钟

    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = LED0_GPIO_Pin | LED1_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);//初始化
    
    GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_Pin | LED1_GPIO_Pin);//GPIOF9,F10设置高，灯灭
}

//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              LED任务
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
        OSTimeDlyHMSM ( 0, 0, 0, 50, OS_OPT_TIME_DLY, & err );  //每500ms扫描一次
        
        
        
	
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
//  @Author             郝wh
//  @brief              创建LED任务
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void LED_OSTaskCreate(void)
{
    OS_ERR  err;
    OSTaskCreate((OS_TCB     *) &LEDTaskTCB,                                //任务控制块地址
                 (CPU_CHAR   *)" LED Task Start",                          //任务名称
                 (OS_TASK_PTR ) LED_Task,                                   //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) LED_TASK_PRIO,                              //任务的优先级
                 (CPU_STK    *)&LEDTaskStk[0],                              //任务堆栈的基地址
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE / 10,                     //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE,                          //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 0u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
}


