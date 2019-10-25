#include "Key.h"
//---------------------------------������ƿ�
static  OS_TCB   Key_PostTaskTCB;                               //key����������ƿ�
static  OS_TCB   Key_PendTaskTCB;                               //key����������ƿ�
//---------------------------------�����ջ
static  CPU_STK  Key_PostTaskStk[Key_Post_TASK_STK_SIZE];       //key���������ջ
static  CPU_STK  Key_PendTaskStk[Key_Pend_TASK_STK_SIZE];       //key���������ջ
//---------------------------------��������
void KEY_Init(void);
static void Key_PostTask(void);
static void Key_Pend_Task(void);
void Key_OSTaskCreate(void);



//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              ������ʼ��
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
} 

//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              ���������ź�
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        ��������������Ϣ����ͨ�ţ���Ϊ��Ϣ����Я����Ϣ�϶�(�ŵ�)
//                      �ఴ�������У���ʹ�������ź���ͨ�ţ��ô��������������(ȱ��)
//------------------------------------------------------------------------------------
static void Key_PostTask(void)
{
    OS_ERR  err;
    static CPU_INT08U Key0_D2U_Flag=0;      //  ����0���ּ���־λ
    static CPU_INT08U Key1_D2U_Flag=0;      //  ����1���ּ���־λ
    for(;;)
    {
        if(KEY0==0)
        {
            OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );    //20ms����
            if(KEY0==0&&Key0_D2U_Flag==0)
            {
//                /* ���������ź��� */
//                OSTaskSemPost(  (OS_TCB  *)&Key_PendTaskTCB,           //Ŀ������
//                                (OS_OPT   )OS_OPT_POST_NONE,           //ûѡ��Ҫ��
//                                (OS_ERR  *)&err);                      //���ش�������
                /* ������Ϣ������ AppTaskPend */
                OSTaskQPost (   (OS_TCB      *)&Key_PendTaskTCB,         //Ŀ������Ŀ��ƿ�
                                (void        *)"Right",                 //��Ϣ����
                                (OS_MSG_SIZE  )sizeof ("Right"),        //��Ϣ����
                                (OS_OPT       )OS_OPT_POST_FIFO,         //���͵�������Ϣ���е���ڶ�
                                (OS_ERR      *)&err);                    //���ش�������
                Key0_D2U_Flag=1;
            }
        }
        else
            Key0_D2U_Flag=0;
        if(KEY1==0)
        {
            OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );    //20ms����
            if(KEY1==0&&Key1_D2U_Flag==0)
            {
//                /* ���������ź��� */
//                OSTaskSemPost(  (OS_TCB  *)&Key_PendTaskTCB,           //Ŀ������
//                                (OS_OPT   )OS_OPT_POST_NONE,           //ûѡ��Ҫ��
//                                (OS_ERR  *)&err);                      //���ش�������
                /* ������Ϣ������ AppTaskPend */
                OSTaskQPost (   (OS_TCB      *)&Key_PendTaskTCB,         //Ŀ������Ŀ��ƿ�
                                (void        *)"Left",                 //��Ϣ����
                                (OS_MSG_SIZE  )sizeof ("Left"),        //��Ϣ����
                                (OS_OPT       )OS_OPT_POST_FIFO,         //���͵�������Ϣ���е���ڶ�
                                (OS_ERR      *)&err);                    //���ش�������
                Key1_D2U_Flag=1;
            }
        }
        else
            Key1_D2U_Flag=0;
        OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );        //ÿ20msɨ��һ��
    }
}
//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              ����ִ��ָ��
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        ��������������Ϣ����ͨ�ţ���Ϊ��Ϣ����Я����Ϣ�϶�(�ŵ�)
//                      �ఴ�������У���ʹ�������ź���ͨ�ţ��ô��������������(ȱ��)
//                      ע�����ȼ���ת����(����ӻ�����)
//------------------------------------------------------------------------------------
static void Key_Pend_Task(void)
{
    OS_ERR         err;
    CPU_TS         ts;
    OS_MSG_SIZE    msg_size;
    
    char * pMsg;
    
    for(;;)
    {
//        OSTaskSemPend (  (OS_TICK   )0,                     //�����޵ȴ�
//                         (OS_OPT    )OS_OPT_PEND_BLOCKING,  //����ź��������þ͵ȴ�
//                         (CPU_TS   *)&ts,                   //��ȡ�ź�����������ʱ���
//                         (OS_ERR   *)&err);                 //���ش�
        /* �������񣬵ȴ�������Ϣ */
		pMsg = OSTaskQPend (  (OS_TICK        )0,                    //�����޵ȴ�
                              (OS_OPT         )OS_OPT_PEND_BLOCKING, //�������� ע�⣺��ֹ���ȼ���ת
                              (OS_MSG_SIZE   *)&msg_size,            //������Ϣ����
                              (CPU_TS        *)&ts,                  //������Ϣ�����͵�ʱ���
                              (OS_ERR        *)&err);                //���ش�������
        if(strcmp(pMsg,"Left")==0)
            LED0_TOGGLE
        else if(strcmp(pMsg,"Right")==0)
            LED1_TOGGLE
        
    }
}
//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              �������񴴽�
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        
//------------------------------------------------------------------------------------
void Key_OSTaskCreate(void)
{
    OS_ERR  err;
    OSTaskCreate((OS_TCB     *) &Key_PostTaskTCB,                          //������ƿ��ַ
                 (CPU_CHAR   *)"Key_Post Task Start",                    //��������
                 (OS_TASK_PTR ) Key_PostTask,                              //������
                 (void       *) 0,                                         //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) Key_Post_TASK_PRIO,                        //��������ȼ�
                 (CPU_STK    *) &Key_PostTaskStk[0],                       //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) Key_Post_TASK_STK_SIZE / 10,               //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) Key_Post_TASK_STK_SIZE,                    //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 0u,                                        //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                        //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                         //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//����ѡ��
                 (OS_ERR     *) &err);                                     //���ش�������
                 
    OSTaskCreate((OS_TCB     *) &Key_PendTaskTCB,                          //������ƿ��ַ
                 (CPU_CHAR   *)"Key_Pend Task Start",                     //��������
                 (OS_TASK_PTR ) Key_Pend_Task,                              //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) Key_Pend_TASK_PRIO,                         //��������ȼ�
                 (CPU_STK    *) &Key_PendTaskStk[0],                        //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) Key_Pend_TASK_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) Key_Pend_TASK_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
}

