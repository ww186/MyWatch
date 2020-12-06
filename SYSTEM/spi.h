#ifndef _SPI_H
#define _SPI_H
#include "sys.h"

void SpiInit(void);
void StartSpi(SPI_TypeDef* SPIx);
void SpiSendData(u8* Data, u16 DataSize);
void SpiReadData(u8* oData, u16 DataSize);
void EndSpi(SPI_TypeDef* SPIx);

void Test(void);

#endif

