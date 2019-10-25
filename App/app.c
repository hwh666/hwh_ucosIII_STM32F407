#include <includes.h>

//---------------------------------������ƿ�
static  OS_TCB   AppTaskStartTCB;      //������ƿ�
//---------------------------------�����ջ
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];       //�����ջ
//---------------------------------��������
static  void  AppTaskStart  (void *p_arg);               //����������


int main(void)
{
    OS_ERR  err;
 
    OSInit(&err);                       //��ʼ�� uC/OS-III                                                 
    /* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) AppTaskStart,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&AppTaskStartStk[0],                         //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 0u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������

    OSStart(&err); 
}

//------------------------------------------------------------------------------------
//  @Author             ��wh
//  @brief              ������������
//  @param              void
//  @return             void 
//  @since              v1.0
//  @Description        ���������ʼ����CPU��ʼ���������ʼ��
//------------------------------------------------------------------------------------
RCC_ClocksTypeDef get_rcc_clock;    //��ȡϵͳʱ��״̬
static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
    
	(void)p_arg;
    
    BSP_Init();                                                 //�弶��ʼ��
    CPU_Init();                                                 //��ʼ�� CPU �����ʱ��������ж�ʱ���������������
    RCC_GetClocksFreq(&get_rcc_clock);  //��ȡʱ��
    cpu_clk_freq = BSP_CPU_ClkFreq();                           //��ȡ CPU �ں�ʱ��Ƶ�ʣ�SysTick ����ʱ�ӣ�
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        //�����û��趨��ʱ�ӽ���Ƶ�ʼ��� SysTick ��ʱ���ļ���ֵ
    OS_CPU_SysTickInit(cnts);                                   //���� SysTick ��ʼ�����������ö�ʱ������ֵ��������ʱ��

    Mem_Init();                                                 //��ʼ���ڴ������������ڴ�غ��ڴ�ر�

#if OS_CFG_STAT_TASK_EN > 0u                                    //���ʹ�ܣ�Ĭ��ʹ�ܣ���ͳ������
    OSStatTaskCPUUsageInit(&err);                               //����û��Ӧ������ֻ�п�����������ʱ CPU �ģ����
#endif                                                          //���������� OS_Stat_IdleCtrMax ��ֵ��Ϊ������� CPU 
                                                                //ʹ����ʹ�ã���
    CPU_IntDisMeasMaxCurReset();                                //��λ�����㣩��ǰ�����ж�ʱ��
    
//    memset(&Time_Cost, 0, sizeof(_Time_Cost));
//    memset(&TimeMeasure_t, 0, sizeof(_TimeMeasure_t));
    Task_SchedulerCreate();
    LED_OSTaskCreate();
    //Task_Tmr_OSTaskCreate();
    Key_OSTaskCreate();
    OSTaskDel( &AppTaskStartTCB, &err );                        //ɾ����ʼ������������������
}

