#ifndef _DEV_EXFLASH_H
#define _DEV_EXFLASH_H
#include "sys.h"

void InitW25qxx(void);
u16 W25QXX_ReadID(void);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);

#endif












