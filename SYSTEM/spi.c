#include "spi.h"
#include "dma.h"
#include "usart.h"

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
	
	//SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_Init(SPI2, &SPI_InitStruct);
	
}

void StartSpi(SPI_TypeDef* SPIx)
{
	SPI_Cmd(SPIx, ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
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
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, DISABLE);
	SPI_Cmd(SPI2,DISABLE);
}









