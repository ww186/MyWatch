#include "spi.h"
#include "dma.h"
#include "usart.h"



static void ResetSpixTxERxE(SPI_TypeDef* SPIx)
{
	//u8 retry=0;
	u8 RecvDataBuffer_t[10];
	u8 SendBuffer_t[10];
	
	SPI_Cmd(SPIx, ENABLE);
	SendBuffer_t[0] = 0xff;
	WriteDataToDma(SendBuffer_t, 1);
	//SPI_I2S_SendData(SPIx, 0xffff);
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) != RESET);
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		//retry++;
		//if(retry>200)return ;
	}	
	printf("1\r\n");
	//SPI_I2S_SendData(SPIx, 0xffff);
	WriteDataToDma(SendBuffer_t, 1);
//	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
//		retry++;
//		if(retry>200)return ;
	}	  		
	
	//SPI_I2S_ReceiveData(SPIx);
	printf("2\r\n");
	ReadDataFromDma(RecvDataBuffer_t, 1);
	SPI_Cmd(SPIx,DISABLE);
}


void SpiInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStruct;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_Init(SPI2, &SPI_InitStruct);
	
	
	//ResetSpixTxERxE(SPI2);
	
	//SPI_Cmd(SPI2,DISABLE);
}

void StartSpi(SPI_TypeDef* SPIx)
{
	SPI_Cmd(SPIx, ENABLE);
	
}

void SpiSendData(u8* Data, u16 DataSize)
{
	
	WriteDataToDma(Data, DataSize);
}

void SpiReadData(u8* oData, u16 DataSize)
{
	
	ReadDataFromDma(oData, DataSize);
					 	
}

void EndSpi(SPI_TypeDef* SPIx)
{
	u8 retry=0;
	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return ;
	}	
	
	retry=0;
	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return ;
	}	
	
	SPI_Cmd(SPI2,DISABLE);
}









