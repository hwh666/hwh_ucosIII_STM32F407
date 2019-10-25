#include "Task_Scheduler.h"

//---------------------------------任务控制块
static  OS_TCB   TaskTCB_2ms;                           //2ms任务控制块
static  OS_TCB   TaskTCB_5ms;                           //5ms任务控制块
static  OS_TCB   TaskTCB_10ms;                           //10ms任务控制块
static  OS_TCB   TaskTCB_20ms;                           //20ms任务控制块
static  OS_TCB   TaskTCB_50ms;                           //50ms任务控制块
//---------------------------------任务堆栈
static  CPU_STK  TaskStk_2ms [TASK_STK_SIZE_2ms];       //2ms任务堆栈
static  CPU_STK  TaskStk_5ms [TASK_STK_SIZE_5ms];       //5ms任务堆栈
static  CPU_STK  TaskStk_10ms[TASK_STK_SIZE_10ms];       //10ms任务堆栈
static  CPU_STK  TaskStk_20ms[TASK_STK_SIZE_20ms];       //20ms任务堆栈
static  CPU_STK  TaskStk_50ms[TASK_STK_SIZE_50ms];       //50ms任务堆栈
//---------------------------------函数声明

static void Task_2ms(void);                             //2ms回调函数
static void Task_5ms(void);                             //5ms回调函数    
static void Task_10ms(void);                            //10ms回调函数
static void Task_20ms(void);                            //20ms回调函数
static void Task_50ms(void);                            //50ms回调函数
void Task_SchedulerCreate(void);                        //任务调度初始化

//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              2ms任务
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------

static void Task_2ms(void)
{
    OS_ERR  err; 
    for(;;)
    {
#if defined TimeMeasurement
    TimeMeasure_t._2ms_Task_T[Old]=OS_TS_GET();
    Time_Cost._2ms_Task_Time.Cycle_T=Get_Cycle_T(_2ms_Task);
    Time_Cost._2ms_Task_Time.Cycle_T_Max=Time_Cost._2ms_Task_Time.Cycle_T>Time_Cost._2ms_Task_Time.Cycle_T_Max?
                                        Time_Cost._2ms_Task_Time.Cycle_T:Time_Cost._2ms_Task_Time.Cycle_T_Max;
#endif
/*----------------------contents of the task---------------------------------*/
	//task here
        LED0_TOGGLE
        OSTimeDlyHMSM ( 0, 0, 0, 2, OS_OPT_TIME_DLY, & err );  //每500ms扫描一次
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t._2ms_Task_T[New]=OS_TS_GET();
    Time_Cost._2ms_Task_Time.Task_T=(TimeMeasure_t._2ms_Task_T[New]-TimeMeasure_t._2ms_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost._2ms_Task_Time.Task_T_Max=Time_Cost._2ms_Task_Time.Task_T>Time_Cost._2ms_Task_Time.Task_T_Max?
                                        Time_Cost._2ms_Task_Time.Task_T:Time_Cost._2ms_Task_Time.Task_T_Max;
#endif
    }
    
}
//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              5ms任务
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------

static void Task_5ms(void)
{
    OS_ERR  err; 
    for(;;)
    {
#if defined TimeMeasurement
    TimeMeasure_t._5ms_Task_T[Old]=OS_TS_GET();
    Time_Cost._5ms_Task_Time.Cycle_T=Get_Cycle_T(_5ms_Task);
    Time_Cost._5ms_Task_Time.Cycle_T_Max=Time_Cost._5ms_Task_Time.Cycle_T>Time_Cost._5ms_Task_Time.Cycle_T_Max?
                                        Time_Cost._5ms_Task_Time.Cycle_T:Time_Cost._5ms_Task_Time.Cycle_T_Max;
#endif
/*----------------------contents of the task---------------------------------*/
	//task here
        LED0_TOGGLE
        OSTimeDlyHMSM ( 0, 0, 0, 5, OS_OPT_TIME_DLY, & err );  //每500ms扫描一次
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t._5ms_Task_T[New]=OS_TS_GET();
    Time_Cost._5ms_Task_Time.Task_T=(TimeMeasure_t._5ms_Task_T[New]-TimeMeasure_t._5ms_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost._5ms_Task_Time.Task_T_Max=Time_Cost._5ms_Task_Time.Task_T>Time_Cost._5ms_Task_Time.Task_T_Max?
                                        Time_Cost._5ms_Task_Time.Task_T:Time_Cost._5ms_Task_Time.Task_T_Max;
#endif
    }
    
}//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              10ms任务
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------

static void Task_10ms(void)
{
    OS_ERR  err; 
    for(;;)
    {
#if defined TimeMeasurement
    TimeMeasure_t._10ms_Task_T[Old]=OS_TS_GET();
    Time_Cost._10ms_Task_Time.Cycle_T=Get_Cycle_T(_10ms_Task);
    Time_Cost._10ms_Task_Time.Cycle_T_Max=Time_Cost._10ms_Task_Time.Cycle_T>Time_Cost._10ms_Task_Time.Cycle_T_Max?
                                        Time_Cost._10ms_Task_Time.Cycle_T:Time_Cost._10ms_Task_Time.Cycle_T_Max;
#endif
/*----------------------contents of the task---------------------------------*/
	//task here
        LED0_TOGGLE
        OSTimeDlyHMSM ( 0, 0, 0, 10, OS_OPT_TIME_DLY, & err );  //每500ms扫描一次
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t._10ms_Task_T[New]=OS_TS_GET();
    Time_Cost._10ms_Task_Time.Task_T=(TimeMeasure_t._10ms_Task_T[New]-TimeMeasure_t._10ms_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost._10ms_Task_Time.Task_T_Max=Time_Cost._10ms_Task_Time.Task_T>Time_Cost._10ms_Task_Time.Task_T_Max?
                                        Time_Cost._10ms_Task_Time.Task_T:Time_Cost._10ms_Task_Time.Task_T_Max;
#endif
    }
    
}//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              20ms任务
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------

static void Task_20ms(void)
{
    OS_ERR  err; 
    for(;;)
    {
#if defined TimeMeasurement
    TimeMeasure_t._20ms_Task_T[Old]=OS_TS_GET();
    Time_Cost._20ms_Task_Time.Cycle_T=Get_Cycle_T(_20ms_Task);
    Time_Cost._20ms_Task_Time.Cycle_T_Max=Time_Cost._20ms_Task_Time.Cycle_T>Time_Cost._20ms_Task_Time.Cycle_T_Max?
                                        Time_Cost._20ms_Task_Time.Cycle_T:Time_Cost._20ms_Task_Time.Cycle_T_Max;
#endif
/*----------------------contents of the task---------------------------------*/
	//task here
        LED0_TOGGLE
        OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );  //每500ms扫描一次
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t._20ms_Task_T[New]=OS_TS_GET();
    Time_Cost._20ms_Task_Time.Task_T=(TimeMeasure_t._20ms_Task_T[New]-TimeMeasure_t._20ms_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost._20ms_Task_Time.Task_T_Max=Time_Cost._20ms_Task_Time.Task_T>Time_Cost._20ms_Task_Time.Task_T_Max?
                                        Time_Cost._20ms_Task_Time.Task_T:Time_Cost._20ms_Task_Time.Task_T_Max;
#endif
    }
    
}//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              50ms任务
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------

static void Task_50ms(void)
{
    OS_ERR  err; 
    for(;;)
    {
#if defined TimeMeasurement
    TimeMeasure_t._50ms_Task_T[Old]=OS_TS_GET();
    Time_Cost._50ms_Task_Time.Cycle_T=Get_Cycle_T(_50ms_Task);
    Time_Cost._50ms_Task_Time.Cycle_T_Max=Time_Cost._50ms_Task_Time.Cycle_T>Time_Cost._50ms_Task_Time.Cycle_T_Max?
                                        Time_Cost._50ms_Task_Time.Cycle_T:Time_Cost._50ms_Task_Time.Cycle_T_Max;
#endif
/*----------------------contents of the task---------------------------------*/
	//task here
        LED0_TOGGLE
        OSTimeDlyHMSM ( 0, 0, 0, 50, OS_OPT_TIME_DLY, & err );  //每500ms扫描一次
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t._50ms_Task_T[New]=OS_TS_GET();
    Time_Cost._50ms_Task_Time.Task_T=(TimeMeasure_t._50ms_Task_T[New]-TimeMeasure_t._50ms_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost._50ms_Task_Time.Task_T_Max=Time_Cost._50ms_Task_Time.Task_T>Time_Cost._50ms_Task_Time.Task_T_Max?
                                        Time_Cost._50ms_Task_Time.Task_T:Time_Cost._50ms_Task_Time.Task_T_Max;
#endif
    }
    
}
//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              创建任务调度
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void Task_SchedulerCreate(void)
{
    OS_ERR  err;
#if Task_2ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_2ms,                                //任务控制块地址
                 (CPU_CHAR   *)" _2ms Task Start",                          //任务名称
                 (OS_TASK_PTR ) Task_2ms,                                   //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) TASK_PRIO_2ms,                              //任务的优先级
                 (CPU_STK    *)&TaskStk_2ms[0],                              //任务堆栈的基地址
                 (CPU_STK_SIZE) TASK_STK_SIZE_2ms / 10,                     //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) TASK_STK_SIZE_2ms,                          //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
#endif
#if Task_5ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_5ms,                                //任务控制块地址
                 (CPU_CHAR   *)" _5ms Task Start",                          //任务名称
                 (OS_TASK_PTR ) Task_5ms,                                   //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) TASK_PRIO_5ms,                              //任务的优先级
                 (CPU_STK    *)&TaskStk_5ms[0],                              //任务堆栈的基地址
                 (CPU_STK_SIZE) TASK_STK_SIZE_5ms / 10,                     //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) TASK_STK_SIZE_5ms,                          //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
#endif
#if Task_10ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_10ms,                                //任务控制块地址
                 (CPU_CHAR   *)" _10ms Task Start",                          //任务名称
                 (OS_TASK_PTR ) Task_10ms,                                   //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) TASK_PRIO_10ms,                              //任务的优先级
                 (CPU_STK    *)&TaskStk_10ms[0],                              //任务堆栈的基地址
                 (CPU_STK_SIZE) TASK_STK_SIZE_10ms / 10,                     //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) TASK_STK_SIZE_10ms,                          //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
#endif
#if Task_20ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_20ms,                                //任务控制块地址
                 (CPU_CHAR   *)" _20ms Task Start",                          //任务名称
                 (OS_TASK_PTR ) Task_20ms,                                   //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) TASK_PRIO_20ms,                              //任务的优先级
                 (CPU_STK    *)&TaskStk_20ms[0],                              //任务堆栈的基地址
                 (CPU_STK_SIZE) TASK_STK_SIZE_20ms / 10,                     //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) TASK_STK_SIZE_20ms,                          //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
#endif
#if Task_50ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_50ms,                                //任务控制块地址
                 (CPU_CHAR   *)" _50ms Task Start",                          //任务名称
                 (OS_TASK_PTR ) Task_50ms,                                   //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) TASK_PRIO_50ms,                              //任务的优先级
                 (CPU_STK    *)&TaskStk_50ms[0],                              //任务堆栈的基地址
                 (CPU_STK_SIZE) TASK_STK_SIZE_50ms / 10,                     //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) TASK_STK_SIZE_50ms,                          //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
#endif
}
