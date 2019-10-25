#include <includes.h>

//---------------------------------任务控制块
static  OS_TCB   AppTaskStartTCB;      //任务控制块
//---------------------------------任务堆栈
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];       //任务堆栈
//---------------------------------函数声明
static  void  AppTaskStart  (void *p_arg);               //任务函数声明


int main(void)
{
    OS_ERR  err;
 
    OSInit(&err);                       //初始化 uC/OS-III                                                 
    /* 创建起始任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //任务控制块地址
                 (CPU_CHAR   *)"App Task Start",                            //任务名称
                 (OS_TASK_PTR ) AppTaskStart,                               //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //任务的优先级
                 (CPU_STK    *)&AppTaskStartStk[0],                         //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 0u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型

    OSStart(&err); 
}

//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              加载所有任务
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        所有外设初始化、CPU初始化、任务初始化
//------------------------------------------------------------------------------------
RCC_ClocksTypeDef get_rcc_clock;    //获取系统时钟状态
static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
    
	(void)p_arg;
    
    BSP_Init();                                                 //板级初始化
    CPU_Init();                                                 //初始化 CPU 组件（时间戳、关中断时间测量和主机名）
    RCC_GetClocksFreq(&get_rcc_clock);  //获取时钟
    cpu_clk_freq = BSP_CPU_ClkFreq();                           //获取 CPU 内核时钟频率（SysTick 工作时钟）
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        //根据用户设定的时钟节拍频率计算 SysTick 定时器的计数值
    OS_CPU_SysTickInit(cnts);                                   //调用 SysTick 初始化函数，设置定时器计数值和启动定时器

    Mem_Init();                                                 //初始化内存管理组件（堆内存池和内存池表）

#if OS_CFG_STAT_TASK_EN > 0u                                    //如果使能（默认使能）了统计任务
    OSStatTaskCPUUsageInit(&err);                               //计算没有应用任务（只有空闲任务）运行时 CPU 的（最大）
#endif                                                          //容量（决定 OS_Stat_IdleCtrMax 的值，为后面计算 CPU 
                                                                //使用率使用）。
    CPU_IntDisMeasMaxCurReset();                                //复位（清零）当前最大关中断时间
    
//    memset(&Time_Cost, 0, sizeof(_Time_Cost));
//    memset(&TimeMeasure_t, 0, sizeof(_TimeMeasure_t));
    Task_SchedulerCreate();
    LED_OSTaskCreate();
    //Task_Tmr_OSTaskCreate();
    Key_OSTaskCreate();
    OSTaskDel( &AppTaskStartTCB, &err );                        //删除起始任务本身，该任务不再运行
}

