#include "ST_nRF24L01_API.h"
#include "ST_Include.h" 


const uchar ST_TX_ADDRESS[ST_TX_ADR_WIDTH]={0,1,2,3,4}; //���͵�ַ
const uchar ST_RX_ADDRESS[ST_RX_ADR_WIDTH]={0x88,0x88,0x88,0x88,0x88}; //���͵�ַ
u8 ST_NRF_TX_BUF[32],ST_NRF_RX_BUF[32];
//#define ST_NRF_CSN            PDout(2)  //Ƭѡ�źţ�������� 
//#define ST_NRF_CE             PBout(3)  //PC5 �������
//#define ST_NRF_SCK  		  PCout(12)  //���ͽ���ģʽѡ���������
//#define ST_NRF_MOSI  		  PCout(11)  //PC6 MOSI ����������� ���������ʳ�ʼ��PC_CR2�Ĵ�����
//#define ST_NRF_MISO  		  PCin(10)
//#define ST_NRF_IRQ  		  PAin(15)  //IRQ������������,��������
void ST_NRF_IO_init(void)
{ 
	 GPIO_InitTypeDef NRF;   //����GPIO��ʼ���ṹ��  �������зֱ����ýṹ���Ա����
	
	
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD,ENABLE);//ʹ��PB��ʱ��
	
	 NRF.GPIO_Mode=GPIO_Mode_OUT; //�������ģʽ
	 NRF.GPIO_Speed=GPIO_Speed_50MHz;//�����������
	 NRF.GPIO_OType=GPIO_OType_PP;
	 NRF.GPIO_PuPd=GPIO_PuPd_UP;
	 NRF.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;	//���������ΪPB0��PB1
	 GPIO_Init(GPIOC,&NRF); //�������涨��Ľṹ���ʼ��GPIOB
	
	 NRF.GPIO_Mode=GPIO_Mode_OUT; //�������ģʽ
	 NRF.GPIO_Speed=GPIO_Speed_50MHz;//�����������
	 NRF.GPIO_OType=GPIO_OType_PP;
	 NRF.GPIO_PuPd=GPIO_PuPd_UP;
	 NRF.GPIO_Pin=GPIO_Pin_3;	//���������ΪPB0��PB1
	 GPIO_Init(GPIOB,&NRF); //�������涨��Ľṹ���ʼ��GPIOB
	
	 NRF.GPIO_Mode=GPIO_Mode_OUT; //�������ģʽ
	 NRF.GPIO_Speed=GPIO_Speed_50MHz;//�����������
	 NRF.GPIO_OType=GPIO_OType_PP;
	 NRF.GPIO_PuPd=GPIO_PuPd_UP;
	 NRF.GPIO_Pin=GPIO_Pin_2;	//���������ΪPB0��PB1
	 GPIO_Init(GPIOD,&NRF); //�������涨��Ľṹ���ʼ��GPIOB
	 
	NRF.GPIO_Pin = GPIO_Pin_10;
	NRF.GPIO_Mode = GPIO_Mode_IN;//����
	NRF.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &NRF);//
	
	NRF.GPIO_Pin = GPIO_Pin_15;
	NRF.GPIO_Mode = GPIO_Mode_IN;//����
	NRF.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &NRF);//
	
	
}

uchar ST_SPI_RW(uchar byte)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // ���8λ
	{
		if((uchar)(byte&0x80)==0x80)
		ST_NRF_MOSI=1; 			// MSB TO MOSI
		else 
			ST_NRF_MOSI=0; 
		byte=(byte<<1);					// shift next bit to MSB
		ST_NRF_SCK=1;
		byte|=ST_NRF_MISO;	        		// capture current MISO bit
		ST_NRF_SCK=0;
	}
	return byte;
}
/*********************************************/
/* �������ܣ���24L01�ļĴ���дֵ��һ���ֽڣ� */
/* ��ڲ�����reg   Ҫд�ļĴ�����ַ          */
/*           value ���Ĵ���д��ֵ            */
/* ���ڲ�����status ״ֵ̬                   */
/*********************************************/
uchar ST_NRF24L01_Write_Reg(uchar reg,uchar value)
{
	uchar status;

	ST_NRF_CSN=0;                  //CSN=0;   
  	status = ST_SPI_RW(reg);		//���ͼĴ�����ַ,����ȡ״ֵ̬
	ST_SPI_RW(value);
	ST_NRF_CSN=1;                  //CSN=1;

	return status;
}
/*************************************************/
/* �������ܣ���24L01�ļĴ���ֵ ��һ���ֽڣ�      */
/* ��ڲ�����reg  Ҫ���ļĴ�����ַ               */
/* ���ڲ�����value �����Ĵ�����ֵ                */
/*************************************************/
uchar ST_NRF24L01_Read_Reg(uchar reg)
{
 	uchar value;

	ST_NRF_CSN=0;              //CSN=0;   
  	ST_SPI_RW(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	value = ST_SPI_RW(ST_NOP);
	ST_NRF_CSN=1;             	//CSN=1;

	return value;
}
/*********************************************/
/* �������ܣ���24L01�ļĴ���ֵ������ֽڣ�   */
/* ��ڲ�����reg   �Ĵ�����ַ                */
/*           *pBuf �����Ĵ���ֵ�Ĵ������    */
/*           len   �����ֽڳ���              */
/* ���ڲ�����status ״ֵ̬                   */
/*********************************************/
uchar ST_NRF24L01_Read_Buf(uchar reg,uchar *pBuf,uchar len)
{
	uchar status,u8_ctr;
	ST_NRF_CSN=0;                   	//CSN=0       
	status=ST_SPI_RW(reg);				//���ͼĴ�����ַ,����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=ST_SPI_RW(0XFF);		//��������
	ST_NRF_CSN=1;                 		//CSN=1
	return status;        			//���ض�����״ֵ̬
}
/**********************************************/
/* �������ܣ���24L01�ļĴ���дֵ������ֽڣ�  */
/* ��ڲ�����reg  Ҫд�ļĴ�����ַ            */
/*           *pBuf ֵ�Ĵ������               */
/*           len   �����ֽڳ���               */
/**********************************************/
uchar ST_NRF24L01_Write_Buf(uchar reg, uchar *pBuf, uchar len)
{
	uchar status,u8_ctr;
	ST_NRF_CSN=0;
	status = ST_SPI_RW(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	ST_SPI_RW(*pBuf++); 				//д������
	ST_NRF_CSN=1;
  return status;          		//���ض�����״ֵ̬
}							  					   

/*********************************************/
/* �������ܣ�24L01��������                   */
/* ��ڲ�����rxbuf ������������              */
/* ����ֵ�� 0   �ɹ��յ�����                 */
/*          1   û���յ�����                 */
/*********************************************/
uchar ST_NRF24L01_RxPacket(uchar *rxbuf)
{
uchar state;
	state=ST_NRF24L01_Read_Reg(ST_STATUS);  			//��ȡ״̬�Ĵ�����ֵ    	 
	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&ST_RX_OK)								//���յ�����
	{
		ST_NRF_CE = 0;
		ST_NRF24L01_Read_Buf(ST_RD_RX_PLOAD,rxbuf,ST_RX_PLOAD_WIDTH);//��ȡ����
		ST_NRF24L01_Write_Reg(ST_FLUSH_RX,0xff);					//���RX FIFO�Ĵ���
		ST_NRF_CE = 1;
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
uchar ST_NRF24L01_TxPacket(uchar *txbuf)
{
	uchar state;
   
	ST_NRF_CE=0;												//CE���ͣ�ʹ��24L01����
  	ST_NRF24L01_Write_Buf(ST_WR_TX_PLOAD,txbuf,ST_TX_PLOAD_WIDTH);	//д���ݵ�TX BUF  32���ֽ�
 	ST_NRF_CE=1;												//CE�øߣ�ʹ�ܷ���	   
	while(ST_NRF_IRQ==1);										//�ȴ��������
	state=ST_NRF24L01_Read_Reg(ST_STATUS);  						//��ȡ״̬�Ĵ�����ֵ	   
	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_STATUS,state); 			//���TX_DS��MAX_RT�жϱ�־

	if(state&ST_MAX_TX)										//�ﵽ����ط�����
	{
		ST_NRF24L01_Write_Reg(ST_FLUSH_TX,0xff);					//���TX FIFO�Ĵ��� 
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
uchar ST_NRF24L01_Check(void)
{
	uchar check_in_buf[5]={0x11,0x22,0x33,0x44,0x55};
	uchar check_out_buf[5]={0x00};

	ST_NRF_SCK=0;
	ST_NRF_CSN=1;    
	ST_NRF_CE=0;
	ST_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_TX_ADDR, check_in_buf, 5);
	ST_NRF24L01_Read_Buf(ST_nRF_READ_REG+ST_TX_ADDR, check_out_buf, 5);	
	
	if((check_out_buf[0] == 0x11)&&\
	   (check_out_buf[1] == 0x22)&&\
	   (check_out_buf[2] == 0x33)&&\
	   (check_out_buf[3] == 0x44)&&\
	   (check_out_buf[4] == 0x55))return 0;
	else return 1;
	
}			


void ST_NRF24L01_RT_Init(void)
{	
	ST_NRF_CE=0;		  
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RX_PW_P0,ST_RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
	ST_NRF24L01_Write_Reg(ST_FLUSH_RX,0xff);									//���RX FIFO�Ĵ���    
  	ST_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_TX_ADDR,(uchar*)ST_TX_ADDRESS,ST_TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	ST_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_RX_ADDR_P0,(uchar*)ST_RX_ADDRESS,ST_RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RF_CH,0);        //����RFͨ��Ϊ2.400GHz  Ƶ��=2.4+0GHz
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RF_SETUP,0x0F);  //����TX�������,0db����,2Mbps,���������濪��   
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_CONFIG,0x0f);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	ST_NRF_CE=1;									  //CE�øߣ�ʹ�ܷ���
}

void ST_NRF_SEND_BUF(uchar *buf)
{
	ST_NRF_CE=0;
	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_CONFIG,0x0e);
	ST_NRF_CE=1;
	ST_Delay_us(30);
	ST_NRF24L01_TxPacket(buf);
	ST_NRF_CE=0;
	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_CONFIG, 0x0f);
	ST_NRF_CE=1;	
}

