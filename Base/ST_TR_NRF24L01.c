#include "ST_TR_NRF24L01.h"
const u8 TR_TX_ADDRESS[ST_TX_ADR_WIDTH]={0x55,0x44,0x33,0x22,0x11}; //���͵�ַ
const u8 TR_RX_ADDRESS[ST_RX_ADR_WIDTH]={12,34,56,78,90};//���͵�ַ
u8 ST_NRF_RX_BUF[32];

//#define USE_HARDWARE_SPI//�Ƿ�ʹ��Ӳ��spi����   ��ʱ��Ӳ��spi����ѧ���⣿����


void ST_TR_NRF_IO_init(void)
{ 
	GPIO_InitTypeDef NRF;   //����GPIO��ʼ���ṹ��  �������зֱ����ýṹ���Ա����

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//ʹ��PB��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//ʹ��PB��ʱ��
	NRF.GPIO_Mode=GPIO_Mode_OUT; //�������ģʽ
	NRF.GPIO_Speed=GPIO_Speed_100MHz;//�����������
	NRF.GPIO_OType=GPIO_OType_PP;
	NRF.GPIO_PuPd=GPIO_PuPd_UP;
	NRF.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;	//���������ΪPB0��PB1
	GPIO_Init(GPIOB,&NRF); //�������涨��Ľṹ���ʼ��GPIOB

	NRF.GPIO_Pin = GPIO_Pin_6;
	NRF.GPIO_Mode = GPIO_Mode_IN;//����
	NRF.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &NRF);//
	
#if defined USE_HARDWARE_SPI	
	ST_SPI2_Init();
#else	
	NRF.GPIO_Mode=GPIO_Mode_OUT; //�������ģʽ
	NRF.GPIO_Speed=GPIO_Speed_100MHz;//�����������
	NRF.GPIO_OType=GPIO_OType_PP;
	NRF.GPIO_PuPd=GPIO_PuPd_UP;
	NRF.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;	//���������ΪPB0��PB1
	GPIO_Init(GPIOB,&NRF); //�������涨��Ľṹ���ʼ��GPIOB

	
	NRF.GPIO_Pin = GPIO_Pin_14;
	NRF.GPIO_Mode = GPIO_Mode_IN;//����
	NRF.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &NRF);//
#endif
}

u8 ST_TR_SPI_RW(u8 byte)
{
#if defined USE_HARDWARE_SPI
	byte=ST_SPI_ReadWriteByte(SPI2,byte);
	return byte;
#else 
	u8 bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // ���8λ
	{
		if((u8)(byte&0x80)==0x80)
		TR_NRF24L01_MOSI=1; 			// MSB TO MOSI
		else 
			TR_NRF24L01_MOSI=0; 
		byte=(byte<<1);					// shift next bit to MSB
		TR_NRF24L01_SCK=1;
		byte|=TR_NRF24L01_MISO;	        		// capture current MISO bit
		TR_NRF24L01_SCK=0;
	}
	return byte;
#endif
}
/*********************************************/
/* �������ܣ���24L01�ļĴ���дֵ��һ���ֽڣ� */
/* ��ڲ�����reg   Ҫд�ļĴ�����ַ          */
/*           value ���Ĵ���д��ֵ            */
/* ���ڲ�����status ״ֵ̬                   */
/*********************************************/
u8 TR_NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;

	TR_NRF24L01_CSN=0;                  //CSN=0;   
  	status = ST_TR_SPI_RW(reg);		//���ͼĴ�����ַ,����ȡ״ֵ̬
	ST_TR_SPI_RW(value);
	TR_NRF24L01_CSN=1;                  //CSN=1;

	return status;
}
/*************************************************/
/* �������ܣ���24L01�ļĴ���ֵ ��һ���ֽڣ�      */
/* ��ڲ�����reg  Ҫ���ļĴ�����ַ               */
/* ���ڲ�����value �����Ĵ�����ֵ                */
/*************************************************/
u8 TR_NRF24L01_Read_Reg(u8 reg)
{
 	u8 value;

	TR_NRF24L01_CSN=0;              //CSN=0;   
  	ST_TR_SPI_RW(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	value = ST_TR_SPI_RW(ST_NOP);
	TR_NRF24L01_CSN=1;             	//CSN=1;

	return value;
}
/*********************************************/
/* �������ܣ���24L01�ļĴ���ֵ������ֽڣ�   */
/* ��ڲ�����reg   �Ĵ�����ַ                */
/*           *pBuf �����Ĵ���ֵ�Ĵ������    */
/*           len   �����ֽڳ���              */
/* ���ڲ�����status ״ֵ̬                   */
/*********************************************/
u8 TR_NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;
	TR_NRF24L01_CSN=0;                   	//CSN=0       
	status=ST_TR_SPI_RW(reg);				//���ͼĴ�����ַ,����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=ST_TR_SPI_RW(0XFF);		//��������
	TR_NRF24L01_CSN=1;                 		//CSN=1
	return status;        			//���ض�����״ֵ̬
}
/**********************************************/
/* �������ܣ���24L01�ļĴ���дֵ������ֽڣ�  */
/* ��ڲ�����reg  Ҫд�ļĴ�����ַ            */
/*           *pBuf ֵ�Ĵ������               */
/*           len   �����ֽڳ���               */
/**********************************************/
u8 TR_NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
	TR_NRF24L01_CSN=0;
	status = ST_TR_SPI_RW(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	ST_TR_SPI_RW(*pBuf++); 				//д������
	TR_NRF24L01_CSN=1;
  return status;          		//���ض�����״ֵ̬
}							  					   

/*********************************************/
/* �������ܣ�24L01��������                   */
/* ��ڲ�����rxbuf ������������              */
/* ����ֵ�� 0   �ɹ��յ�����                 */
/*          1   û���յ�����                 */
/*********************************************/
u8 state;
u8 TR_NRF24L01_RxPacket(u8 *rxbuf)
{

	state=TR_NRF24L01_Read_Reg(ST_STATUS);  			//��ȡ״̬�Ĵ�����ֵ    	 
	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&ST_RX_OK)								//���յ�����
	{
		TR_NRF24L01_CE = 0;
		TR_NRF24L01_Read_Buf(ST_RD_RX_PLOAD,rxbuf,ST_RX_PLOAD_WIDTH);//��ȡ����
		TR_NRF24L01_Write_Reg(ST_FLUSH_RX,0xff);					//���RX FIFO�Ĵ���
		TR_NRF24L01_CE = 1;
		//ST_Delay_us(50);
		return 0; 
	}	   
	return 1;//û�յ��κ�����
	
}
/**********************************************/
/* �������ܣ�����24L01Ϊ����ģʽ              */
/* ��ڲ�����txbuf  ������������              */
/* ����ֵ�� 0x10    �ﵽ����ط�����������ʧ��*/
/*          0x20    �ɹ��������              */
/*          0xff    ����ʧ��                  */
/**********************************************/
u8 TR_NRF24L01_TxPacket(u8 *txbuf)
{
	u8 state;
   
	TR_NRF24L01_CE=0;												//CE���ͣ�ʹ��24L01����
  	TR_NRF24L01_Write_Buf(ST_WR_TX_PLOAD,txbuf,ST_TX_PLOAD_WIDTH);	//д���ݵ�TX BUF  32���ֽ�
 	TR_NRF24L01_CE=1;												//CE�øߣ�ʹ�ܷ���	   
	while(TR_NRF24L01_IRQ==1);										//�ȴ��������
	state=TR_NRF24L01_Read_Reg(ST_STATUS);  						//��ȡ״̬�Ĵ�����ֵ	   
	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_STATUS,state); 			//���TX_DS��MAX_RT�жϱ�־
	if(state&ST_MAX_TX)										//�ﵽ����ط�����
	{
		TR_NRF24L01_Write_Reg(ST_FLUSH_TX,0xff);					//���TX FIFO�Ĵ��� 
		return ST_MAX_TX; 
	}
	if(state&ST_TX_OK)											//�������
	{
		return ST_TX_OK;
	}
	return 0xff;											//����ʧ��
}

/********************************************/
/* �������ܣ����24L01�Ƿ����              */
/* ����ֵ��  0  ����                        */
/*           1  ������                      */
/********************************************/ 	  
u8 TR_NRF24L01_Check(void)
{
	u8 check_in_buf[5]={0x11,0x22,0x33,0x44,0x55};
	u8 check_out_buf[5]={0x00};

	TR_NRF24L01_SCK=0;
	TR_NRF24L01_CSN=1;    
	TR_NRF24L01_CE=0;
	TR_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_TX_ADDR, check_in_buf, 5);
	TR_NRF24L01_Read_Buf(ST_nRF_READ_REG+ST_TX_ADDR, check_out_buf, 5);	
	
	if((check_out_buf[0] == 0x11)&&\
	   (check_out_buf[1] == 0x22)&&\
	   (check_out_buf[2] == 0x33)&&\
	   (check_out_buf[3] == 0x44)&&\
	   (check_out_buf[4] == 0x55))return 0;
	else return 1;
	
}			


void TR_NRF24L01_RT_Init(void)
{	
	TR_NRF24L01_CE=0;		  
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RX_PW_P0,ST_RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
	TR_NRF24L01_Write_Reg(ST_FLUSH_RX,0xff);									//���RX FIFO�Ĵ���    
  	TR_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_TX_ADDR,(u8*)TR_TX_ADDRESS,ST_TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	TR_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_RX_ADDR_P0,(u8*)TR_RX_ADDRESS,ST_TX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RF_CH,50);        //����RFͨ��Ϊ2.400GHz  Ƶ��=2.4+0GHz
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RF_SETUP,0x0F);  //����TX�������,0db����,2Mbps,���������濪��   
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_CONFIG,0x0f);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	TR_NRF24L01_CE=1;									  //CE�øߣ�ʹ�ܷ���
	
	ST_flag.lost=1;
}
void ST_nRF24L01_Init(void)
{
	ST_TR_NRF_IO_init();
	while(TR_NRF24L01_Check());
	TR_NRF24L01_RT_Init();//ң���źŽ���
}


