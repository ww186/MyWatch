#include "dma.h"
#include <stdlib.h>
#include "usart.h"

static u8 Dma1_4Buffer[100];
static u8 Dma1_5Buffer[100];
static DMA_InitTypeDef DMA_InitStruct;

void InitDma(void)
{
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStruct.DMA_BufferSize = 500;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)Dma1_4Buffer;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR);
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);
	
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)Dma1_5Buffer;
//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC | DMA_IT_HT, ENABLE);
//	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC | DMA_IT_HT, ENABLE);
	DMA_Init(DMA1_Channel5, &DMA_InitStruct);
	
	DMA_Cmd(DMA1_Channel5, DISABLE );
	DMA_Cmd(DMA1_Channel4, DISABLE );
}

void WriteDataToDma(u8* WriteData, u16 DataSize)
{
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_ClearFlag(DMA1_FLAG_TC5);
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)WriteData;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Init(DMA1_Channel5, &DMA_InitStruct);
	
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)Dma1_4Buffer;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);
	
	DMA_SetCurrDataCounter(DMA1_Channel5, DataSize);
	DMA_SetCurrDataCounter(DMA1_Channel4, DataSize);
	DMA_Cmd(DMA1_Channel5, ENABLE );
	DMA_Cmd(DMA1_Channel4, ENABLE );
	while(DMA_GetFlagStatus(DMA1_FLAG_TC5)==RESET);
	
	DMA_Cmd(DMA1_Channel5, DISABLE );
	DMA_Cmd(DMA1_Channel4, DISABLE );
}

void ReadDataFromDma(u8* ReadData, u16 DataSize)
{
	Dma1_5Buffer[0] = 0xff;
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_ClearFlag(DMA1_FLAG_TC5);
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)ReadData;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);
	
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)Dma1_5Buffer;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Init(DMA1_Channel5, &DMA_InitStruct);
	
	DMA_SetCurrDataCounter(DMA1_Channel5, DataSize);
	DMA_SetCurrDataCounter(DMA1_Channel4, DataSize);
	DMA_Cmd(DMA1_Channel5, ENABLE );
	DMA_Cmd(DMA1_Channel4, ENABLE );
	while(DMA_GetFlagStatus(DMA1_FLAG_TC5)==RESET);
	
	DMA_Cmd(DMA1_Channel5, DISABLE );
	DMA_Cmd(DMA1_Channel4, DISABLE );
	
}

//void DMA1_Channel5_IRQHandler(void)
//{
//	printf("1\r\n");
//}

//void DMA1_Channel4_IRQHandler(void)
//{
//	printf("2\r\n");
//}
