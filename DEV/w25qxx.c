#include "Dev_ExFlash.h"
#include "spi.h"
#include "usart.h"

//ָ���
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
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
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
	W25qxxSendByte(W25X_ReadStatusReg); //���Ͷ�ȡ״̬�Ĵ�������    
	byte=W25qxxReadByte();          //��ȡһ���ֽ�  
	DisableW25qxxCS();                            //ȡ��Ƭѡ     
	return byte;   
} 

static void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);  		// �ȴ�BUSYλ���
} 

void W25QXX_Write_SR(u8 sr)   
{   
	EnableW25qxxCS();                            //ʹ������   
	W25qxxSendByte(W25X_WriteStatusReg);//����дȡ״̬�Ĵ�������    
	W25qxxSendByte(sr);               	//д��һ���ֽ�  
	DisableW25qxxCS();                           //ȡ��Ƭѡ     	      
}

void W25QXX_Write_Enable(void)   
{
	EnableW25qxxCS();                          	//ʹ������   
    W25qxxSendByte(W25X_WriteEnable); 	//����дʹ��  
	DisableW25qxxCS();                          	//ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
//void W25QXX_Write_Disable(void)   
//{  
//	EnableW25qxxCS();                           //ʹ������   
//    W25qxxSendByte(W25X_WriteDisable);  //����д��ָֹ��    
//	DisableW25qxxCS();                           //ȡ��Ƭѡ     	      
//} 


u16 W25QXX_ReadID(void)
{
	u8 SendData_t[] = {0x90, 0x00, 0x00, 0x00};
	u8 RecvData_t[10];
	EnableW25qxxCS();				    
	W25qxxSendData(SendData_t, 4);//���Ͷ�ȡID����	
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
	EnableW25qxxCS();                            	//ʹ������   
  W25qxxSendData(SendData_t, 4);
	
	W25qxxRecvData(pBuffer, NumByteToRead);

	DisableW25qxxCS();  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u8 SendData_t[10];
	SendData_t[0] = W25X_PageProgram;
	SendData_t[1] = (u8)((WriteAddr)>>16);
	SendData_t[2] = (u8)((WriteAddr)>>8);
	SendData_t[3] = (u8)WriteAddr;
  W25QXX_Write_Enable();                  	//SET WEL 
	EnableW25qxxCS();                            	//ʹ������   
  W25qxxSendData(SendData_t, 4);  
	W25qxxSendData(pBuffer, NumByteToWrite);
	DisableW25qxxCS();                             	//ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   		//�ȴ�д�����
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
	W25QXX_Write_Enable();                            	//ʹ������   
	EnableW25qxxCS();
	W25qxxSendData(SendData_t, 4); 
	DisableW25qxxCS();                          	//ȡ��Ƭѡ     	      
  W25QXX_Wait_Busy();   				   		//�ȴ��������
}  

void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
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
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF) break;//��Ҫ����  			
		}
		
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);		//�����������
			for(i=0;i<secremain;i++)	   		//����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  				//ָ��ƫ��
			WriteAddr+=secremain;				//д��ַƫ��	   
		   	NumByteToWrite-=secremain;			//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;//��һ����������д����
			else secremain=NumByteToWrite;		//��һ����������д����
		}	 
	};	 
}



