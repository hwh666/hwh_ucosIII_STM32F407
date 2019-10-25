#ifndef __nRF24L01_API_H
#define __nRF24L01_API_H

#include "ST_nRF24L01_soft.h"
#define uchar unsigned char
#define uint unsigned int


#define ST_NRF_CSN            PDout(2)  //片选信号，推挽输出 
#define ST_NRF_CE             PBout(3)  //PC5 推完输出
#define ST_NRF_SCK  		  PCout(12)  //发送接收模式选择推挽输出
#define ST_NRF_MOSI  		  PCout(11)  //PC6 MOSI 主机推挽输出 （根据速率初始化PC_CR2寄存器）
#define ST_NRF_MISO  		  PCin(10)
#define ST_NRF_IRQ  		  PAin(15)  //IRQ主机数据输入,上拉输入


void ST_NRF_IO_init(void);
uchar ST_SPI_RW(uchar byte);
uchar ST_NRF24L01_Write_Reg(uchar reg,uchar value);
uchar ST_NRF24L01_Read_Reg(uchar reg);
uchar ST_NRF24L01_Read_Buf(uchar reg,uchar *pBuf,uchar len);
uchar ST_NRF24L01_Write_Buf(uchar reg, uchar *pBuf, uchar len);
uchar ST_NRF24L01_RxPacket(uchar *rxbuf);
uchar ST_NRF24L01_TxPacket(uchar *txbuf);
uchar ST_NRF24L01_Check(void);
void ST_NRF24L01_RT_Init(void);
void ST_NRF_SEND_BUF(uchar *buf);
extern uchar  ST_NRF_TX_BUF[32],ST_NRF_RX_BUF[32];
#endif
