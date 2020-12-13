#include "Dev_ExFlash.h"
#include "spi.h"
#include "usart.h"

//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


void InitW25qxx(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	
}

static void EnableW25qxxCS(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	StartSpi(SPI2);
}

static void DisableW25qxxCS(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	EndSpi(SPI2);
}

static void W25qxxSendData(u8* Data, u16 DataSize)
{
	SpiSendData(Data, DataSize);
}

static void W25qxxRecvData(u8* oData, u16 DataSize)
{
	SpiReadData(oData, DataSize);
}

static void W25qxxSendByte(u8 Data)
{
	u8 Data_t;
	
	Data_t = Data;
	SpiSendData(&Data_t, 1);
}

static u8 W25qxxReadByte(void)
{
	u8 Data_t;
	
	SpiReadData(&Data_t, 1);
	return Data_t;
}


u8 W25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	EnableW25qxxCS();
	W25qxxSendByte(W25X_ReadStatusReg); //发送读取状态寄存器命令    
	byte=W25qxxReadByte();          //读取一个字节  
	DisableW25qxxCS();                            //取消片选     
	return byte;   
} 

static void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);  		// 等待BUSY位清空
} 

void W25QXX_Write_SR(u8 sr)   
{   
	EnableW25qxxCS();                            //使能器件   
	W25qxxSendByte(W25X_WriteStatusReg);//发送写取状态寄存器命令    
	W25qxxSendByte(sr);               	//写入一个字节  
	DisableW25qxxCS();                           //取消片选     	      
}

void W25QXX_Write_Enable(void)   
{
	EnableW25qxxCS();                          	//使能器件   
    W25qxxSendByte(W25X_WriteEnable); 	//发送写使能  
	DisableW25qxxCS();                          	//取消片选     	      
} 
//W25QXX写禁止	
//将WEL清零  
//void W25QXX_Write_Disable(void)   
//{  
//	EnableW25qxxCS();                           //使能器件   
//    W25qxxSendByte(W25X_WriteDisable);  //发送写禁止指令    
//	DisableW25qxxCS();                           //取消片选     	      
//} 


u16 W25QXX_ReadID(void)
{
	u8 SendData_t[] = {0x90, 0x00, 0x00, 0x00};
	u8 RecvData_t[10];
	EnableW25qxxCS();				    
	W25qxxSendData(SendData_t, 4);//发送读取ID命令	
	W25qxxRecvData(RecvData_t, 2);
	DisableW25qxxCS();				    
	return (u16)RecvData_t[0]<<8 | RecvData_t[1];
}

void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 

	u8 SendData_t[10];
	SendData_t[0] = W25X_ReadData;
	SendData_t[1] = (u8)((ReadAddr)>>16);
	SendData_t[2] = (u8)((ReadAddr)>>8);
	SendData_t[3] = (u8)ReadAddr;
	EnableW25qxxCS();                            	//使能器件   
  W25qxxSendData(SendData_t, 4);
	
	W25qxxRecvData(pBuffer, NumByteToRead);

	DisableW25qxxCS();  				    	      
}  
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u8 SendData_t[10];
	SendData_t[0] = W25X_PageProgram;
	SendData_t[1] = (u8)((WriteAddr)>>16);
	SendData_t[2] = (u8)((WriteAddr)>>8);
	SendData_t[3] = (u8)WriteAddr;
  W25QXX_Write_Enable();                  	//SET WEL 
	EnableW25qxxCS();                            	//使能器件   
  W25qxxSendData(SendData_t, 4);  
	W25qxxSendData(pBuffer, NumByteToWrite);
	DisableW25qxxCS();                             	//取消片选 
	W25QXX_Wait_Busy();					   		//等待写入结束
}

static void W25QXX_Erase_Sector(u16 Sector)   
{  
	u32  Dst_Addr; 
	u8 SendData_t[10];
	Dst_Addr=Sector*4096;
	
	SendData_t[0] = W25X_SectorErase;
	SendData_t[1] = (u8)((Dst_Addr)>>16);
	SendData_t[2] = (u8)((Dst_Addr)>>8);
	SendData_t[3] = (u8)Dst_Addr;	
 	
//	W25QXX_Write_Enable();                  	//SET WEL 	 
//	W25QXX_Wait_Busy();   
	W25QXX_Write_Enable();                            	//使能器件   
	EnableW25qxxCS();
	W25qxxSendData(SendData_t, 4); 
	DisableW25qxxCS();                          	//取消片选     	      
  W25QXX_Wait_Busy();   				   		//等待擦除完成
}  

void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 

static u8 W25QXX_BUFFER[4096];		
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF) break;//需要擦除  			
		}
		
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	};	 
}



