#include "ST_nRF24L01_API.h"
#include "ST_Include.h" 


const uchar ST_TX_ADDRESS[ST_TX_ADR_WIDTH]={0,1,2,3,4}; //发送地址
const uchar ST_RX_ADDRESS[ST_RX_ADR_WIDTH]={0x88,0x88,0x88,0x88,0x88}; //发送地址
u8 ST_NRF_TX_BUF[32],ST_NRF_RX_BUF[32];
//#define ST_NRF_CSN            PDout(2)  //片选信号，推挽输出 
//#define ST_NRF_CE             PBout(3)  //PC5 推完输出
//#define ST_NRF_SCK  		  PCout(12)  //发送接收模式选择推挽输出
//#define ST_NRF_MOSI  		  PCout(11)  //PC6 MOSI 主机推挽输出 （根据速率初始化PC_CR2寄存器）
//#define ST_NRF_MISO  		  PCin(10)
//#define ST_NRF_IRQ  		  PAin(15)  //IRQ主机数据输入,上拉输入
void ST_NRF_IO_init(void)
{ 
	 GPIO_InitTypeDef NRF;   //定义GPIO初始化结构体  下面三行分别设置结构体成员变量
	
	
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD,ENABLE);//使能PB口时钟
	
	 NRF.GPIO_Mode=GPIO_Mode_OUT; //设置输出模式
	 NRF.GPIO_Speed=GPIO_Speed_50MHz;//设置输出速率
	 NRF.GPIO_OType=GPIO_OType_PP;
	 NRF.GPIO_PuPd=GPIO_PuPd_UP;
	 NRF.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;	//设置输出口为PB0和PB1
	 GPIO_Init(GPIOC,&NRF); //按照上面定义的结构体初始化GPIOB
	
	 NRF.GPIO_Mode=GPIO_Mode_OUT; //设置输出模式
	 NRF.GPIO_Speed=GPIO_Speed_50MHz;//设置输出速率
	 NRF.GPIO_OType=GPIO_OType_PP;
	 NRF.GPIO_PuPd=GPIO_PuPd_UP;
	 NRF.GPIO_Pin=GPIO_Pin_3;	//设置输出口为PB0和PB1
	 GPIO_Init(GPIOB,&NRF); //按照上面定义的结构体初始化GPIOB
	
	 NRF.GPIO_Mode=GPIO_Mode_OUT; //设置输出模式
	 NRF.GPIO_Speed=GPIO_Speed_50MHz;//设置输出速率
	 NRF.GPIO_OType=GPIO_OType_PP;
	 NRF.GPIO_PuPd=GPIO_PuPd_UP;
	 NRF.GPIO_Pin=GPIO_Pin_2;	//设置输出口为PB0和PB1
	 GPIO_Init(GPIOD,&NRF); //按照上面定义的结构体初始化GPIOB
	 
	NRF.GPIO_Pin = GPIO_Pin_10;
	NRF.GPIO_Mode = GPIO_Mode_IN;//输入
	NRF.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &NRF);//
	
	NRF.GPIO_Pin = GPIO_Pin_15;
	NRF.GPIO_Mode = GPIO_Mode_IN;//输入
	NRF.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &NRF);//
	
	
}

uchar ST_SPI_RW(uchar byte)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // 输出8位
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
/* 函数功能：给24L01的寄存器写值（一个字节） */
/* 入口参数：reg   要写的寄存器地址          */
/*           value 给寄存器写的值            */
/* 出口参数：status 状态值                   */
/*********************************************/
uchar ST_NRF24L01_Write_Reg(uchar reg,uchar value)
{
	uchar status;

	ST_NRF_CSN=0;                  //CSN=0;   
  	status = ST_SPI_RW(reg);		//发送寄存器地址,并读取状态值
	ST_SPI_RW(value);
	ST_NRF_CSN=1;                  //CSN=1;

	return status;
}
/*************************************************/
/* 函数功能：读24L01的寄存器值 （一个字节）      */
/* 入口参数：reg  要读的寄存器地址               */
/* 出口参数：value 读出寄存器的值                */
/*************************************************/
uchar ST_NRF24L01_Read_Reg(uchar reg)
{
 	uchar value;

	ST_NRF_CSN=0;              //CSN=0;   
  	ST_SPI_RW(reg);			//发送寄存器值(位置),并读取状态值
	value = ST_SPI_RW(ST_NOP);
	ST_NRF_CSN=1;             	//CSN=1;

	return value;
}
/*********************************************/
/* 函数功能：读24L01的寄存器值（多个字节）   */
/* 入口参数：reg   寄存器地址                */
/*           *pBuf 读出寄存器值的存放数组    */
/*           len   数组字节长度              */
/* 出口参数：status 状态值                   */
/*********************************************/
uchar ST_NRF24L01_Read_Buf(uchar reg,uchar *pBuf,uchar len)
{
	uchar status,u8_ctr;
	ST_NRF_CSN=0;                   	//CSN=0       
	status=ST_SPI_RW(reg);				//发送寄存器地址,并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=ST_SPI_RW(0XFF);		//读出数据
	ST_NRF_CSN=1;                 		//CSN=1
	return status;        			//返回读到的状态值
}
/**********************************************/
/* 函数功能：给24L01的寄存器写值（多个字节）  */
/* 入口参数：reg  要写的寄存器地址            */
/*           *pBuf 值的存放数组               */
/*           len   数组字节长度               */
/**********************************************/
uchar ST_NRF24L01_Write_Buf(uchar reg, uchar *pBuf, uchar len)
{
	uchar status,u8_ctr;
	ST_NRF_CSN=0;
	status = ST_SPI_RW(reg);			//发送寄存器值(位置),并读取状态值
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	ST_SPI_RW(*pBuf++); 				//写入数据
	ST_NRF_CSN=1;
  return status;          		//返回读到的状态值
}							  					   

/*********************************************/
/* 函数功能：24L01接收数据                   */
/* 入口参数：rxbuf 接收数据数组              */
/* 返回值： 0   成功收到数据                 */
/*          1   没有收到数据                 */
/*********************************************/
uchar ST_NRF24L01_RxPacket(uchar *rxbuf)
{
uchar state;
	state=ST_NRF24L01_Read_Reg(ST_STATUS);  			//读取状态寄存器的值    	 
	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_STATUS,state); //清除TX_DS或MAX_RT中断标志
	if(state&ST_RX_OK)								//接收到数据
	{
		ST_NRF_CE = 0;
		ST_NRF24L01_Read_Buf(ST_RD_RX_PLOAD,rxbuf,ST_RX_PLOAD_WIDTH);//读取数据
		ST_NRF24L01_Write_Reg(ST_FLUSH_RX,0xff);					//清除RX FIFO寄存器
		ST_NRF_CE = 1;
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
uchar ST_NRF24L01_TxPacket(uchar *txbuf)
{
	uchar state;
   
	ST_NRF_CE=0;												//CE拉低，使能24L01配置
  	ST_NRF24L01_Write_Buf(ST_WR_TX_PLOAD,txbuf,ST_TX_PLOAD_WIDTH);	//写数据到TX BUF  32个字节
 	ST_NRF_CE=1;												//CE置高，使能发送	   
	while(ST_NRF_IRQ==1);										//等待发送完成
	state=ST_NRF24L01_Read_Reg(ST_STATUS);  						//读取状态寄存器的值	   
	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_STATUS,state); 			//清除TX_DS或MAX_RT中断标志

	if(state&ST_MAX_TX)										//达到最大重发次数
	{
		ST_NRF24L01_Write_Reg(ST_FLUSH_TX,0xff);					//清除TX FIFO寄存器 
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
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RX_PW_P0,ST_RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
	ST_NRF24L01_Write_Reg(ST_FLUSH_RX,0xff);									//清除RX FIFO寄存器    
  	ST_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_TX_ADDR,(uchar*)ST_TX_ADDRESS,ST_TX_ADR_WIDTH);//写TX节点地址 
  	ST_NRF24L01_Write_Buf(ST_nRF_WRITE_REG+ST_RX_ADDR_P0,(uchar*)ST_RX_ADDRESS,ST_RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_EN_AA,0x01);     //使能通道0的自动应答    
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_EN_RXADDR,0x01); //使能通道0的接收地址  
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RF_CH,0);        //设置RF通道为2.400GHz  频率=2.4+0GHz
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_RF_SETUP,0x0F);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	ST_NRF24L01_Write_Reg(ST_nRF_WRITE_REG+ST_CONFIG,0x0f);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	ST_NRF_CE=1;									  //CE置高，使能发送
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

