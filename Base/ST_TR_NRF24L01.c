#include "ST_TR_NRF24L01.h"
const u8 TR_TX_ADDRESS[ST_TX_ADR_WIDTH]={0x55,0x44,0x33,0x22,0x11}; //发送地址
const u8 TR_RX_ADDRESS[ST_RX_ADR_WIDTH]={12,34,56,78,90};//发送地址
u8 ST_NRF_RX_BUF[32];

//#define USE_HARDWARE_SPI//是否使用硬件spi开关   有时候硬件spi有玄学问题？？？


void ST_TR_NRF_IO_init(void)
{ 
	GPIO_InitTypeDef NRF;   //定义GPIO初始化结构体  下面三行分别设置结构体成员变量

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//使能PB口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//使能PB口时钟
	NRF.GPIO_Mode=GPIO_Mode_OUT; //设置输出模式
	NRF.GPIO_Speed=GPIO_Speed_100MHz;//设置输出速率
	NRF.GPIO_OType=GPIO_OType_PP;
	NRF.GPIO_PuPd=GPIO_PuPd_UP;
	NRF.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;	//设置输出口为PB0和PB1
	GPIO_Init(GPIOB,&NRF); //按照上面定义的结构体初始化GPIOB

	NRF.GPIO_Pin = GPIO_Pin_6;
	NRF.GPIO_Mode = GPIO_Mode_IN;//输入
	NRF.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &NRF);//
	
#if defined USE_HARDWARE_SPI	
	ST_SPI2_Init();
#else	
	NRF.GPIO_Mode=GPIO_Mode_OUT; //设置输出模式
	NRF.GPIO_Speed=GPIO_Speed_100MHz;//设置输出速率
	NRF.GPIO_OType=GPIO_OType_PP;
	NRF.GPIO_PuPd=GPIO_PuPd_UP;
	NRF.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;	//设置输出口为PB0和PB1
	GPIO_Init(GPIOB,&NRF); //按照上面定义的结构体初始化GPIOB

	
	NRF.GPIO_Pin = GPIO_Pin_14;
	NRF.GPIO_Mode = GPIO_Mode_IN;//输入
	NRF.GPIO_PuPd = GPIO_PuPd_UP;//上拉
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
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // 输出8位
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
/* 函数功能：给24L01的寄存器写值（一个字节） */
/* 入口参数：reg   要写的寄存器地址          */
/*           value 给寄存器写的值            */
/* 出口参数：status 状态值                   */
/*********************************************/
u8 TR_NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;

	TR_NRF24L01_CSN=0;                  //CSN=0;   
  	status = ST_TR_SPI_RW(reg);		//发送寄存器地址,并读取状态值
	ST_TR_SPI_RW(value);
	TR_NRF24L01_CSN=1;                  //CSN=1;

	return status;
}
/*************************************************/
/* 函数功能：读24L01的寄存器值 （一个字节）      */
/* 入口参数：reg  要读的寄存器地址               */
/* 出口参数：value 读出寄存器的值                */
/*************************************************/
u8 TR_NRF24L01_Read_Reg(u8 reg)
{
 	u8 value;

	TR_NRF24L01_CSN=0;              //CSN=0;   
  	ST_TR_SPI_RW(reg);			//发送寄存器值(位置),并读取状态值
	value = ST_TR_SPI_RW(ST_NOP);
	TR_NRF24L01_CSN=1;             	//CSN=1;

	return value;
}
/*********************************************/
/* 函数功能：读24L01的寄存器值（多个字节）   */
/* 入口参数：reg   寄存器地址                */
/*           *pBuf 读出寄存器值的存放数组    */
/*           len   数组字节长度              */
/* 出口参数：status 状态值                   */
/*********************************************/
u8 TR_NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;
	TR_NRF24L01_CSN=0;                   	//CSN=0       
	status=ST_TR_SPI_RW(reg);				//发送寄存器地址,并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=ST_TR_SPI_RW(0XFF);		//读出数据
	TR_NRF24L01_CSN=1;                 		//CSN=1
	return status;        			//返回读到的状态值
}
/**********************************************/
/* 函数功能：给24L01的寄存器写值（多个字节）  */
/* 入口参数：reg  要写的寄存器地址            */
/*           *pBuf 值的存放数组               */
/*           len   数组字节长度               */
/**********************************************/
u8 TR_NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
	TR_NRF24L01_CSN=0;
	status = ST_TR_SPI_RW(reg);			//发送寄存器值(位置),并读取状态值
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	ST_TR_SPI_RW(*pBuf++); 				//写入数据
	TR_NRF24L01_CSN=1;
  return status;          		//返回读到的状态值
}							  					   

/*********************************************/
/* 函数功能：24L01接收数据                   */
/* 入口参数：rxbuf 接收数据数组              */
/* 返回值： 0   成功收到数据                 */
/*          1   没有收到数据                 */
/*********************************************/
u8 state;
u8 TR_NRF24L01_RxPacket(u8 *rxbuf)
{

	state=TR_NRF24L01_Read_Reg(ST_STATUS);  			//读取状态寄存器的值    	 
	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_STATUS,state); //清除TX_DS或MAX_RT中断标志
	if(state&ST_RX_OK)								//接收到数据
	{
		TR_NRF24L01_CE = 0;
		TR_NRF24L01_Read_Buf(ST_RD_RX_PLOAD,rxbuf,ST_RX_PLOAD_WIDTH);//读取数据
		TR_NRF24L01_Write_Reg(ST_FLUSH_RX,0xff);					//清除RX FIFO寄存器
		TR_NRF24L01_CE = 1;
		//ST_Delay_us(50);
		return 0; 
	}	   
	return 1;//没收到任何数据
	
}
/**********************************************/
/* 函数功能：设置24L01为发送模式              */
/* 入口参数：txbuf  发送数据数组              */
/* 返回值； 0x10    达到最大重发次数，发送失败*/
/*          0x20    成功发送完成              */
/*          0xff    发送失败                  */
/**********************************************/
u8 TR_NRF24L01_TxPacket(u8 *txbuf)
{
	u8 state;
   
	TR_NRF24L01_CE=0;												//CE拉低，使能24L01配置
  	TR_NRF24L01_Write_Buf(ST_WR_TX_PLOAD,txbuf,ST_TX_PLOAD_WIDTH);	//写数据到TX BUF  32个字节
 	TR_NRF24L01_CE=1;												//CE置高，使能发送	   
	while(TR_NRF24L01_IRQ==1);										//等待发送完成
	state=TR_NRF24L01_Read_Reg(ST_STATUS);  						//读取状态寄存器的值	   
	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_STATUS,state); 			//清除TX_DS或MAX_RT中断标志
	if(state&ST_MAX_TX)										//达到最大重发次数
	{
		TR_NRF24L01_Write_Reg(ST_FLUSH_TX,0xff);					//清除TX FIFO寄存器 
		return ST_MAX_TX; 
	}
	if(state&ST_TX_OK)											//发送完成
	{
		return ST_TX_OK;
	}
	return 0xff;											//发送失败
}

/********************************************/
/* 函数功能：检测24L01是否存在              */
/* 返回值；  0  存在                        */
/*           1  不存在                      */
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
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RX_PW_P0,ST_RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
	TR_NRF24L01_Write_Reg(ST_FLUSH_RX,0xff);									//清除RX FIFO寄存器    
  	TR_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_TX_ADDR,(u8*)TR_TX_ADDRESS,ST_TX_ADR_WIDTH);//写TX节点地址 
  	TR_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_RX_ADDR_P0,(u8*)TR_RX_ADDRESS,ST_TX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_EN_AA,0x01);     //使能通道0的自动应答    
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_EN_RXADDR,0x01); //使能通道0的接收地址  
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RF_CH,50);        //设置RF通道为2.400GHz  频率=2.4+0GHz
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RF_SETUP,0x0F);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	TR_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_CONFIG,0x0f);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	TR_NRF24L01_CE=1;									  //CE置高，使能发送
	
	ST_flag.lost=1;
}
void ST_nRF24L01_Init(void)
{
	ST_TR_NRF_IO_init();
	while(TR_NRF24L01_Check());
	TR_NRF24L01_RT_Init();//遥控信号接收
}


