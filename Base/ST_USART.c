#include "ST_USART.h"	
#include <includes.h>
	
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((ST_Printf_Use_USARTx->SR&0X40)==0);//ѭ������,ֱ���������   ��ɵ��
	ST_Printf_Use_USARTx->DR = (u8) ch;      
	return ch;
}
/******************************************************************************/
/*                    ���ڳ�ʼ��������װ			                          */
/******************************************************************************/
void ST_SerialCom_Init(u8 num,u32 baud)
{
	switch (num)
    {
    	case 1: ST_USART1_Init(baud);
    		break;
    	case 2: ST_USART2_Init(baud);
    		break;
		case 3: ST_USART3_Init(baud);
    		break;
		case 4: ST_UART4_Init(baud);
    		break;
		case 5: ST_UART5_Init(baud);
    		break;
    	default:ST_USART1_Init(baud);
    		break;
    }
}

/******************************************************************************/
/*                    ����1��ʼ�����жϷ��񡢷��ͺ���                         */
/******************************************************************************/
	

//��ʼ��IO ����1 
//bound:������
void ST_USART1_Init(u32 bound)
{
   //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����


	
}


u8 Tx1Buffer[256];
u8 Tx1Counter=0;
u8 count1=0; 
void ST_USART1_IRQ(void) 
{
	u8 com_data;
	//ORE�ж�
	if(USART1->SR & USART_SR_ORE)
	{
		com_data = USART1->DR;
	}

  //�����ж�
	if( USART_GetITStatus(USART1,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//����жϱ�־

		com_data = USART1->DR;
	    //ANO_DT_Data_Receive_Prepare(com_data);
		
		
		//here is the data decode 
		
		
	}
	//���ͣ�������λ���ж�
	if( USART_GetITStatus(USART1,USART_IT_TXE ) )
	{
				
		USART1->DR = Tx1Buffer[Tx1Counter++]; //дDR����жϱ�־          
		if(Tx1Counter == count1)
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�������жϣ��ж�
		}
	}
}

void ST_USART1_Send(unsigned char *DataToSend ,u8 data_num)
{
    u8 i;
    OS_ERR  err;
    OSSchedLock(&err);
	for(i=0;i<data_num;i++)
	{
		Tx1Buffer[count1++] = *(DataToSend+i);
	}

	if(!(USART1->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //�򿪷����ж�
	}
    OSSchedUnlock(&err);

}
/******************************************************************************/
/*                    ����2��ʼ�����жϷ��񡢷��ͺ���                         */
/******************************************************************************/
void ST_USART2_Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //����USART2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
//	//����PA2��ΪUSART2��Tx
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//����PA3��ΪUSART2��Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//����USART2
	//�жϱ�������
	USART_InitStructure.USART_BaudRate = br_num;       //�����ʿ���ͨ������վ����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx;  //����ʹ��
	
	
	
	//����USART2ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
	
	USART_Init(USART2, &USART_InitStructure);
	USART_ClockInit(USART2, &USART_ClockInitStruct);

	//ʹ��USART2�����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//ʹ��USART2
	USART_Cmd(USART2, ENABLE);
}

u8 Tx2Buffer[256];
u8 Tx2Counter=0;
u8 count2=0; 
void ST_USART2_IRQ(void)
{
	u8 com_data;
	
	if(USART2->SR & USART_SR_ORE)//ORE�ж�
	{
		com_data = USART2->DR;
	}

  //�����ж�
	if( USART_GetITStatus(USART2,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//����жϱ�־

		com_data = USART2->DR;
		
		//here is the data decode 
	    // USART_SendData(USART2,com_data);
	}
	//���ͣ�������λ���ж�
	if( USART_GetITStatus(USART2,USART_IT_TXE ) )
	{
				
		USART2->DR = Tx2Buffer[Tx2Counter++]; //дDR����жϱ�־          
		if(Tx2Counter == count2)
		{
			USART2->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�������жϣ��ж�
		}
	}
}
void ST_USART2_Send(unsigned char *DataToSend ,u8 data_num)
{
  u8 i;
    OS_ERR  err;
    OSSchedLock(&err);
	for(i=0;i<data_num;i++)
	{
		Tx2Buffer[count2++] = *(DataToSend+i);
	}

	if(!(USART2->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE); //�򿪷����ж�
	}
    OSSchedUnlock(&err);

}
/******************************************************************************/
/*                    ����3��ʼ�����жϷ��񡢷��ͺ���                         */
/******************************************************************************/
void ST_USART3_Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //����USART3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);	
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	
	//����PB10��ΪUSART3��Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
	//����PB11��ΪUSART3��Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	//����USART3
	//�жϱ�������
	USART_InitStructure.USART_BaudRate = br_num;       //�����ʿ���ͨ������վ����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	//����USART3ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
	
	USART_Init(USART3, &USART_InitStructure);
	USART_ClockInit(USART3, &USART_ClockInitStruct);

	//ʹ��USART3�����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//ʹ��USART3
	USART_Cmd(USART3, ENABLE);
}

u8 Tx3Buffer[256];
u8 Tx3Counter=0;
u8 count3=0; 
void ST_USART3_IRQ(void)
{
	u8 com_data;
	
	if(USART3->SR & USART_SR_ORE)//ORE�ж�
	{
		com_data = USART3->DR;
	}

  //�����ж�
	if( USART_GetITStatus(USART3,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//����жϱ�־

		com_data = USART3->DR;
		
		//here is the data decode 

	}
	//���ͣ�������λ���ж�
	if( USART_GetITStatus(USART3,USART_IT_TXE ) )
	{
				
		USART3->DR = Tx3Buffer[Tx3Counter++]; //дDR����жϱ�־          
		if(Tx3Counter == count3)
		{
			USART3->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�������жϣ��ж�
		}
	}
}
void ST_USART3_Send(unsigned char *DataToSend ,u8 data_num)
{
  u8 i;
    OS_ERR  err;
    OSSchedLock(&err);
	for(i=0;i<data_num;i++)
	//for(i=1;i<data_num;i++)
	{
		Tx3Buffer[count3++] = *(DataToSend+i);
	}

	if(!(USART3->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //�򿪷����ж�
	}
    OSSchedUnlock(&err);

}
/******************************************************************************/
/*                    ����4��ʼ�����жϷ��񡢷��ͺ���                         */
/******************************************************************************/
void ST_UART4_Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); //����USART2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	

	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

	//����PC10��ΪUART4��Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//����PC11��ΪUART4��Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	//����UART4
	//�жϱ�������
	USART_InitStructure.USART_BaudRate = br_num;       //�����ʿ���ͨ������վ����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	USART_Init(UART4, &USART_InitStructure);
	


	//ʹ��UART4�����ж�
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//ʹ��USART5
	USART_Cmd(UART4, ENABLE); 


}
u8 Tx4Buffer[256];
u8 Tx4Counter=0;
u8 count4=0; 

void ST_UART4_IRQ(void)
{
	u8 com_data;

  //�����ж�
	if( USART_GetITStatus(UART4,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);//����жϱ�־

		com_data = UART4->DR;
		
		//your code here(com_data);
	}

	//���ͣ�������λ���ж�
	if( USART_GetITStatus(UART4,USART_IT_TXE ) )
	{
				
		UART4->DR = Tx4Buffer[Tx4Counter++]; //дDR����жϱ�־
          
		if(Tx4Counter == count4)
		{
			UART4->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�������жϣ��ж�
		}
	}

}

void ST_UART4_Send(unsigned char *DataToSend ,u8 data_num)
{
	u8 i;
    OS_ERR  err;
    OSSchedLock(&err);
	for(i=0;i<data_num;i++)
	{
		Tx4Buffer[count4++] = *(DataToSend+i);
	}

	if(!(UART4->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(UART4, USART_IT_TXE, ENABLE); //�򿪷����ж�
	}
    OSSchedUnlock(&err);

}


/******************************************************************************/
/*                    ����5��ʼ�����жϷ��񡢷��ͺ���                         */
/******************************************************************************/
void ST_UART5_Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //����USART2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

	//����PC12��ΪUART5��Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	//����PD2��ΪUART5��Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	//����UART5
	//�жϱ�������
	USART_InitStructure.USART_BaudRate = br_num;       //�����ʿ���ͨ������վ����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	USART_Init(UART5, &USART_InitStructure);
	


	//ʹ��UART5�����ж�
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	//ʹ��USART5
	USART_Cmd(UART5, ENABLE); 


}
u8 Tx5Buffer[256];
u8 Tx5Counter=0;
u8 count5=0; 

void ST_UART5_IRQ(void)
{
	u8 com_data;
    
  //�����ж�
	if( USART_GetITStatus(UART5,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);//����жϱ�־

		com_data = UART5->DR;
		//USART_SendData(UART5,com_data);
		//your code here(com_data);
	}

	//���ͣ�������λ���ж�
	if( USART_GetITStatus(UART5,USART_IT_TXE ) )
	{
				
		UART5->DR = Tx5Buffer[Tx5Counter++]; //дDR����жϱ�־
          
		if(Tx5Counter == count5)
		{
			UART5->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�������жϣ��ж�
		}
	}

}

void ST_UART5_Send(unsigned char *DataToSend ,u8 data_num)
{
	u8 i;
    OS_ERR  err;
    OSSchedLock(&err);
	for(i=0;i<data_num;i++)
	{
		Tx5Buffer[count5++] = *(DataToSend+i);
	}

	if(!(UART5->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(UART5, USART_IT_TXE, ENABLE); //�򿪷����ж�
	}
    OSSchedUnlock(&err);
}

