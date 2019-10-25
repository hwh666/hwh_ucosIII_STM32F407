#include "Task_Tmr.h"

//---------------------------------任务控制块
static  OS_TCB   Task_TmrTaskTCB;                           //任务控制块
//---------------------------------任务堆栈
static  CPU_STK  Task_TmrStk[Task_Tmr_STK_SIZE];       //任务堆栈
//---------------------------------函数声明
static void TmrCallback (OS_TMR *p_tmr, void *p_arg) ;
void Task_Tmr_OSTaskCreate(void);
static  void  AppTaskTmr ( void * p_arg );


static void TmrCallback (OS_TMR *p_tmr, void *p_arg) //软件定时器MyTmr的回调函数
{
    LED0_TOGGLE
}
void Task_Tmr_OSTaskCreate(void)
{
    OS_ERR      err;

    /* 创建 AppTaskTmr 任务 */
    OSTaskCreate((OS_TCB     *)&Task_TmrTaskTCB,                             //任务控制块地址
                 (CPU_CHAR   *)"App Task Tmr",                             //任务名称
                 (OS_TASK_PTR ) AppTaskTmr,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) Task_Tmr_TASK_PRIO,                         //任务的优先级
                 (CPU_STK    *)&Task_TmrStk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) Task_Tmr_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) Task_Tmr_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
                    
}
static  void  AppTaskTmr ( void * p_arg )
{
	OS_ERR      err;
	OS_TMR      my_tmr;   //声明软件定时器对象

	
	(void)p_arg;


    /* 创建软件定时器 */
    OSTmrCreate (  (OS_TMR              *)&my_tmr,             //软件定时器对象
                   (CPU_CHAR            *)"MySoftTimer",       //命名软件定时器
                   (OS_TICK              )10,                  //定时器初始值，依10Hz时基计算，即为1s
                   (OS_TICK              )10,                  //定时器周期重载值，依10Hz时基计算，即为1s
                   (OS_OPT               )OS_OPT_TMR_PERIODIC, //周期性定时
                   (OS_TMR_CALLBACK_PTR  )TmrCallback,         //回调函数
                   (void                *)"Timer Over!",       //传递实参给回调函数
                   (OS_ERR              *)err);                //返回错误类型
							 
	/* 启动软件定时器 */						 
    OSTmrStart ((OS_TMR   *)&my_tmr, //软件定时器对象
                (OS_ERR   *)err);    //返回错误类型
				
    for(;;) //任务体，通常写成一个死循环
    {
        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //不断阻塞该任务
    }                   	
}

