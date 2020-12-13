/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include "Dev_ExFlash.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


#define FLASH_SECTOR_SIZE 	512	
#define FLASH_BLOCK_SIZE   	8     	//每个BLOCK有8个扇区
#define FLASH_SECTOR_COUNT  2048*12


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
static DSTATUS RAM_disk_status(void)
{
	return STA_NOINIT;
}

static DSTATUS MMC_disk_status(void)
{
	return RES_OK;
}

static DSTATUS USB_disk_status(void)
{
	return STA_NOINIT;
}

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
	case DEV_RAM :
		stat = RAM_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		stat = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		stat = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/


static DSTATUS RAM_disk_initialize(void)
{
	return STA_NOINIT;
}

static DSTATUS MMC_disk_initialize(void)
{
	InitW25qxx();
	return RES_OK;
}

static DSTATUS USB_disk_initialize(void)
{
	return STA_NOINIT;
}

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
	case DEV_RAM :
		stat = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		stat = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		stat = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
static DRESULT RAM_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
	return RES_NOTRDY;
}

static DRESULT MMC_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
		for(;count>0;count--)
		{
			W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
			sector++;
			buff+=FLASH_SECTOR_SIZE;
		}
		
		return RES_OK;
}

static DRESULT USB_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
	return RES_NOTRDY;
}

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		res = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		res = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		res = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

static DRESULT RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return RES_NOTRDY;
}

static DRESULT MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	for(;count>0;count--)
	{										    
		W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
		sector++;
		buff+=FLASH_SECTOR_SIZE;
	}
	return RES_OK;
}

static DRESULT USB_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return RES_NOTRDY;
}

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		res = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		res = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		res = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive
		res = RES_NOTRDY;
		return res;

	case DEV_MMC :

		// Process of the command for the MMC/SD card
		
		switch(cmd)
		{
			case CTRL_SYNC:
			res = RES_OK; 
					break;	 
			case GET_SECTOR_SIZE:
					*(WORD*)buff = FLASH_SECTOR_SIZE;
					res = RES_OK;
					break;	 
			case GET_BLOCK_SIZE:
					*(WORD*)buff = FLASH_BLOCK_SIZE;
					res = RES_OK;
					break;	 
			case GET_SECTOR_COUNT:
					*(DWORD*)buff = FLASH_SECTOR_COUNT;
					res = RES_OK;
					break;
			default:
					res = RES_PARERR;
					break;
		}
    return res;

	case DEV_USB :

		// Process of the command the USB drive
		res = RES_NOTRDY;
		return res;
	}

	return RES_PARERR;
}

