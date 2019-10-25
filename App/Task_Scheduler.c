#include "Task_Scheduler.h"

//---------------------------------������ƿ�
static  OS_TCB   TaskTCB_2ms;                           //2ms������ƿ�
static  OS_TCB   TaskTCB_5ms;                           //5ms������ƿ�
static  OS_TCB   TaskTCB_10ms;                           //10ms������ƿ�
static  OS_TCB   TaskTCB_20ms;                           //20ms������ƿ�
static  OS_TCB   TaskTCB_50ms;                           //50ms������ƿ�
//---------------------------------�����ջ
static  CPU_STK  TaskStk_2ms [TASK_STK_SIZE_2ms];       //2ms�����ջ
static  CPU_STK  TaskStk_5ms [TASK_STK_SIZE_5ms];       //5ms�����ջ
static  CPU_STK  TaskStk_10ms[TASK_STK_SIZE_10ms];       //10ms�����ջ
static  CPU_STK  TaskStk_20ms[TASK_STK_SIZE_20ms];       //20ms�����ջ
static  CPU_STK  TaskStk_50ms[TASK_STK_SIZE_50ms];       //50ms�����ջ
//---------------------------------��������

static void Task_2ms(void);                             //2ms�ص�����
static void Task_5ms(void);                             //5ms�ص�����    
static void Task_10ms(void);                            //10ms�ص�����
static void Task_20ms(void);                            //20ms�ص�����
static void Task_50ms(void);                            //50ms�ص�����
void Task_SchedulerCreate(void);                        //������ȳ�ʼ��

//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              2ms����
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
        OSTimeDlyHMSM ( 0, 0, 0, 2, OS_OPT_TIME_DLY, & err );  //ÿ500msɨ��һ��
        
        
        
	
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
//  @Author             ��wh
//  @brief              5ms����
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
        OSTimeDlyHMSM ( 0, 0, 0, 5, OS_OPT_TIME_DLY, & err );  //ÿ500msɨ��һ��
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t._5ms_Task_T[New]=OS_TS_GET();
    Time_Cost._5ms_Task_Time.Task_T=(TimeMeasure_t._5ms_Task_T[New]-TimeMeasure_t._5ms_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost._5ms_Task_Time.Task_T_Max=Time_Cost._5ms_Task_Time.Task_T>Time_Cost._5ms_Task_Time.Task_T_Max?
                                        Time_Cost._5ms_Task_Time.Task_T:Time_Cost._5ms_Task_Time.Task_T_Max;
#endif
    }
    
}//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              10ms����
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
        OSTimeDlyHMSM ( 0, 0, 0, 10, OS_OPT_TIME_DLY, & err );  //ÿ500msɨ��һ��
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t._10ms_Task_T[New]=OS_TS_GET();
    Time_Cost._10ms_Task_Time.Task_T=(TimeMeasure_t._10ms_Task_T[New]-TimeMeasure_t._10ms_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost._10ms_Task_Time.Task_T_Max=Time_Cost._10ms_Task_Time.Task_T>Time_Cost._10ms_Task_Time.Task_T_Max?
                                        Time_Cost._10ms_Task_Time.Task_T:Time_Cost._10ms_Task_Time.Task_T_Max;
#endif
    }
    
}//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              20ms����
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
        OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );  //ÿ500msɨ��һ��
        
        
        
	
/*---------------------------------------------------------------------------*/        
#if defined TimeMeasurement	
    TimeMeasure_t._20ms_Task_T[New]=OS_TS_GET();
    Time_Cost._20ms_Task_Time.Task_T=(TimeMeasure_t._20ms_Task_T[New]-TimeMeasure_t._20ms_Task_T[Old])/( get_rcc_clock.HCLK_Frequency / 1000000);
    Time_Cost._20ms_Task_Time.Task_T_Max=Time_Cost._20ms_Task_Time.Task_T>Time_Cost._20ms_Task_Time.Task_T_Max?
                                        Time_Cost._20ms_Task_Time.Task_T:Time_Cost._20ms_Task_Time.Task_T_Max;
#endif
    }
    
}//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              50ms����
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
        OSTimeDlyHMSM ( 0, 0, 0, 50, OS_OPT_TIME_DLY, & err );  //ÿ500msɨ��һ��
        
        
        
	
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
//  @Author             ��wh
//  @brief              �����������
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void Task_SchedulerCreate(void)
{
    OS_ERR  err;
#if Task_2ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_2ms,                                //������ƿ��ַ
                 (CPU_CHAR   *)" _2ms Task Start",                          //��������
                 (OS_TASK_PTR ) Task_2ms,                                   //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) TASK_PRIO_2ms,                              //��������ȼ�
                 (CPU_STK    *)&TaskStk_2ms[0],                              //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) TASK_STK_SIZE_2ms / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) TASK_STK_SIZE_2ms,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
#endif
#if Task_5ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_5ms,                                //������ƿ��ַ
                 (CPU_CHAR   *)" _5ms Task Start",                          //��������
                 (OS_TASK_PTR ) Task_5ms,                                   //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) TASK_PRIO_5ms,                              //��������ȼ�
                 (CPU_STK    *)&TaskStk_5ms[0],                              //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) TASK_STK_SIZE_5ms / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) TASK_STK_SIZE_5ms,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
#endif
#if Task_10ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_10ms,                                //������ƿ��ַ
                 (CPU_CHAR   *)" _10ms Task Start",                          //��������
                 (OS_TASK_PTR ) Task_10ms,                                   //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) TASK_PRIO_10ms,                              //��������ȼ�
                 (CPU_STK    *)&TaskStk_10ms[0],                              //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) TASK_STK_SIZE_10ms / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) TASK_STK_SIZE_10ms,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
#endif
#if Task_20ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_20ms,                                //������ƿ��ַ
                 (CPU_CHAR   *)" _20ms Task Start",                          //��������
                 (OS_TASK_PTR ) Task_20ms,                                   //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) TASK_PRIO_20ms,                              //��������ȼ�
                 (CPU_STK    *)&TaskStk_20ms[0],                              //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) TASK_STK_SIZE_20ms / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) TASK_STK_SIZE_20ms,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
#endif
#if Task_50ms_EN > 0u
    OSTaskCreate((OS_TCB     *) &TaskTCB_50ms,                                //������ƿ��ַ
                 (CPU_CHAR   *)" _50ms Task Start",                          //��������
                 (OS_TASK_PTR ) Task_50ms,                                   //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) TASK_PRIO_50ms,                              //��������ȼ�
                 (CPU_STK    *)&TaskStk_50ms[0],                              //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) TASK_STK_SIZE_50ms / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) TASK_STK_SIZE_50ms,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
#endif
}
