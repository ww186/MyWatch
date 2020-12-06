#ifndef _DMA_H
#define _DMA_H
#include "sys.h"

void InitDma(void);
void WriteDataToDma(u8* WriteData, u16 DataSize);
void ReadDataFromDma(u8* ReadData, u16 DataSize);
//void WriteDataToDma(u8* Data, u16 DataSize);
//void ReadDataFromDma(u8* oData, u16 DataSize);



#endif


