#include "Key.h"
//---------------------------------任务控制块
static  OS_TCB   Key_PostTaskTCB;                               //key发送任务控制块
static  OS_TCB   Key_PendTaskTCB;                               //key接收任务控制块
//---------------------------------任务堆栈
static  CPU_STK  Key_PostTaskStk[Key_Post_TASK_STK_SIZE];       //key发送任务堆栈
static  CPU_STK  Key_PendTaskStk[Key_Pend_TASK_STK_SIZE];       //key接收任务堆栈
//---------------------------------函数声明
void KEY_Init(void);
static void Key_PostTask(void);
static void Key_Pend_Task(void);
void Key_OSTaskCreate(void);



//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              按键初始化
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0 KEY1 KEY2对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP对应引脚PA0
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
} 

//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              按键触发信号
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        按键采用任务消息队列通信，因为消息队列携带信息较多(优点)
//                      多按键触发中，若使用任务信号量通信，得创建多个按键任务(缺点)
//------------------------------------------------------------------------------------
static void Key_PostTask(void)
{
    OS_ERR  err;
    static CPU_INT08U Key0_D2U_Flag=0;      //  按键0松手检测标志位
    static CPU_INT08U Key1_D2U_Flag=0;      //  按键1松手检测标志位
    for(;;)
    {
        if(KEY0==0)
        {
            OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );    //20ms消抖
            if(KEY0==0&&Key0_D2U_Flag==0)
            {
//                /* 发布任务信号量 */
//                OSTaskSemPost(  (OS_TCB  *)&Key_PendTaskTCB,           //目标任务
//                                (OS_OPT   )OS_OPT_POST_NONE,           //没选项要求
//                                (OS_ERR  *)&err);                      //返回错误类型
                /* 发送消息到任务 AppTaskPend */
                OSTaskQPost (   (OS_TCB      *)&Key_PendTaskTCB,         //目标任务的控制块
                                (void        *)"Right",                 //消息内容
                                (OS_MSG_SIZE  )sizeof ("Right"),        //消息长度
                                (OS_OPT       )OS_OPT_POST_FIFO,         //发送到任务消息队列的入口端
                                (OS_ERR      *)&err);                    //返回错误类型
                Key0_D2U_Flag=1;
            }
        }
        else
            Key0_D2U_Flag=0;
        if(KEY1==0)
        {
            OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );    //20ms消抖
            if(KEY1==0&&Key1_D2U_Flag==0)
            {
//                /* 发布任务信号量 */
//                OSTaskSemPost(  (OS_TCB  *)&Key_PendTaskTCB,           //目标任务
//                                (OS_OPT   )OS_OPT_POST_NONE,           //没选项要求
//                                (OS_ERR  *)&err);                      //返回错误类型
                /* 发送消息到任务 AppTaskPend */
                OSTaskQPost (   (OS_TCB      *)&Key_PendTaskTCB,         //目标任务的控制块
                                (void        *)"Left",                 //消息内容
                                (OS_MSG_SIZE  )sizeof ("Left"),        //消息长度
                                (OS_OPT       )OS_OPT_POST_FIFO,         //发送到任务消息队列的入口端
                                (OS_ERR      *)&err);                    //返回错误类型
                Key1_D2U_Flag=1;
            }
        }
        else
            Key1_D2U_Flag=0;
        OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );        //每20ms扫描一次
    }
}
//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              按键执行指令
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        按键采用任务消息队列通信，因为消息队列携带信息较多(优点)
//                      多按键触发中，若使用任务信号量通信，得创建多个按键任务(缺点)
//                      注意优先级翻转问题(可添加互斥量)
//------------------------------------------------------------------------------------
static void Key_Pend_Task(void)
{
    OS_ERR         err;
    CPU_TS         ts;
    OS_MSG_SIZE    msg_size;
    
    char * pMsg;
    
    for(;;)
    {
//        OSTaskSemPend (  (OS_TICK   )0,                     //无期限等待
//                         (OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果信号量不可用就等待
//                         (CPU_TS   *)&ts,                   //获取信号量被发布的时间戳
//                         (OS_ERR   *)&err);                 //返回错
        /* 阻塞任务，等待任务消息 */
		pMsg = OSTaskQPend (  (OS_TICK        )0,                    //无期限等待
                              (OS_OPT         )OS_OPT_PEND_BLOCKING, //阻塞任务 注意：防止优先级翻转
                              (OS_MSG_SIZE   *)&msg_size,            //返回消息长度
                              (CPU_TS        *)&ts,                  //返回消息被发送的时间戳
                              (OS_ERR        *)&err);                //返回错误类型
        if(strcmp(pMsg,"Left")==0)
            LED0_TOGGLE
        else if(strcmp(pMsg,"Right")==0)
            LED1_TOGGLE
        
    }
}
//------------------------------------------------------------------------------------
//  @Author             郝wh
//  @brief              按键任务创建
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void Key_OSTaskCreate(void)
{
    OS_ERR  err;
    OSTaskCreate((OS_TCB     *) &Key_PostTaskTCB,                          //任务控制块地址
                 (CPU_CHAR   *)"Key_Post Task Start",                    //任务名称
                 (OS_TASK_PTR ) Key_PostTask,                              //任务函数
                 (void       *) 0,                                         //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) Key_Post_TASK_PRIO,                        //任务的优先级
                 (CPU_STK    *) &Key_PostTaskStk[0],                       //任务堆栈的基地址
                 (CPU_STK_SIZE) Key_Post_TASK_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) Key_Post_TASK_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 0u,                                        //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                        //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                         //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//任务选项
                 (OS_ERR     *) &err);                                     //返回错误类型
                 
    OSTaskCreate((OS_TCB     *) &Key_PendTaskTCB,                          //任务控制块地址
                 (CPU_CHAR   *)"Key_Pend Task Start",                     //任务名称
                 (OS_TASK_PTR ) Key_Pend_Task,                              //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) Key_Pend_TASK_PRIO,                         //任务的优先级
                 (CPU_STK    *) &Key_PendTaskStk[0],                        //任务堆栈的基地址
                 (CPU_STK_SIZE) Key_Pend_TASK_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) Key_Pend_TASK_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
}

