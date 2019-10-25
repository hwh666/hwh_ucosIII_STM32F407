#ifndef __SPI
#define __SPI
#include "sys.h"
u8 ST_SPI_ReadWriteByte(SPI_TypeDef * SPIx,u8 TxData);
void ST_SPI2_Init(void);
void ST_SPI1_Init(void);
void ST_SPI_Transmit(SPI_TypeDef * SPIx,uint8_t *pData, uint16_t Size);
void ST_SPI_Receive(SPI_TypeDef * SPIx,uint8_t *pData, uint16_t Size);
#endif


