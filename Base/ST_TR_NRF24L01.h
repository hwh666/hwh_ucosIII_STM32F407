#ifndef __24l01
#define __24l01
#include "ST_Include.h"
#include "ST_nRF24L01_soft.h"
#define TR_NRF24L01_CE   PBout(11) 	//24L01片选信号
#define TR_NRF24L01_CSN  PBout(12) 	//SPI片选信号	   
#define TR_NRF24L01_IRQ  PCin(6)  	//IRQ主机数据输入
#define TR_NRF24L01_SCK   PBout(13) 	//24L01片选信号
#define TR_NRF24L01_MOSI  PBout(15) 	//SPI片选信号	   
#define TR_NRF24L01_MISO  PBin(14)  	//IRQ主机数据输入
							  
							  
void ST_TR_NRF_IO_init(void);
u8 ST_TR_SPI_RW(u8 byte);
u8 TR_NRF24L01_Write_Reg(u8 reg,u8 value);
u8 TR_NRF24L01_Read_Reg(u8 reg);
u8 TR_NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len);
u8 TR_NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len);
u8 TR_NRF24L01_RxPacket(u8 *rxbuf);
u8 TR_NRF24L01_TxPacket(u8 *txbuf);  
u8 TR_NRF24L01_Check(void);
void TR_NRF24L01_RT_Init(void);
void TR_NRF24L01_TR_Init(void);
void TR_NRF24L01_SEND_BUF(u8 *buf);
void ST_nRF24L01_Init(void);

extern u8 ST_NRF_RX_BUF[32];
#endif

