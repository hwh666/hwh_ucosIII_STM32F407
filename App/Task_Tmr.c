#include "Task_Tmr.h"

//---------------------------------������ƿ�
static  OS_TCB   Task_TmrTaskTCB;                           //������ƿ�
//---------------------------------�����ջ
static  CPU_STK  Task_TmrStk[Task_Tmr_STK_SIZE];       //�����ջ
//---------------------------------��������
static void TmrCallback (OS_TMR *p_tmr, void *p_arg) ;
void Task_Tmr_OSTaskCreate(void);
static  void  AppTaskTmr ( void * p_arg );


static void TmrCallback (OS_TMR *p_tmr, void *p_arg) //�����ʱ��MyTmr�Ļص�����
{
    LED0_TOGGLE
}
void Task_Tmr_OSTaskCreate(void)
{
    OS_ERR      err;

    /* ���� AppTaskTmr ���� */
    OSTaskCreate((OS_TCB     *)&Task_TmrTaskTCB,                             //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Tmr",                             //��������
                 (OS_TASK_PTR ) AppTaskTmr,                                //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) Task_Tmr_TASK_PRIO,                         //��������ȼ�
                 (CPU_STK    *)&Task_TmrStk[0],                          //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) Task_Tmr_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) Task_Tmr_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
                    
}
static  void  AppTaskTmr ( void * p_arg )
{
	OS_ERR      err;
	OS_TMR      my_tmr;   //���������ʱ������

	
	(void)p_arg;


    /* ���������ʱ�� */
    OSTmrCreate (  (OS_TMR              *)&my_tmr,             //�����ʱ������
                   (CPU_CHAR            *)"MySoftTimer",       //���������ʱ��
                   (OS_TICK              )10,                  //��ʱ����ʼֵ����10Hzʱ�����㣬��Ϊ1s
                   (OS_TICK              )10,                  //��ʱ����������ֵ����10Hzʱ�����㣬��Ϊ1s
                   (OS_OPT               )OS_OPT_TMR_PERIODIC, //�����Զ�ʱ
                   (OS_TMR_CALLBACK_PTR  )TmrCallback,         //�ص�����
                   (void                *)"Timer Over!",       //����ʵ�θ��ص�����
                   (OS_ERR              *)err);                //���ش�������
							 
	/* ���������ʱ�� */						 
    OSTmrStart ((OS_TMR   *)&my_tmr, //�����ʱ������
                (OS_ERR   *)err);    //���ش�������
				
    for(;;) //�����壬ͨ��д��һ����ѭ��
    {
        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //��������������
    }                   	
}

