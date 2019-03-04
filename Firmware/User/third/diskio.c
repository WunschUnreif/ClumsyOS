/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include "bsp.h"
#include <stdio.h>

/* Definitions of physical drive number for each drive */
#define DEV_SDCARD 0

SD_CardInfo cardinfo;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = 0;
	SD_CardStatus status;

	switch (pdrv) {
	case DEV_SDCARD: 
		if(SD_GetCardStatus(&status) != SD_OK) {
			stat = STA_NOINIT;
		} else {
			stat = 0;
		}
		break;
	default:
		stat = STA_NOINIT;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	
	DSTATUS stat = 0;

	switch (pdrv) {
	case DEV_SDCARD: 
		if(SDcardInit() == 0) {
			stat = STA_NOINIT;
		} else {
			SD_GetCardInfo(&cardinfo);
		}
		break;
	default:
		stat = STA_NOINIT;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	DWORD temp[512 / 4];

	switch (pdrv) {
	case DEV_SDCARD: 
		if((uint32_t)(buff) & 0x03) {
			while(count--) {
				res = disk_read(pdrv, (BYTE*)temp, sector++, 1);
				if(res != RES_OK) {
					break;
				}

				for(uint16_t i = 0; i < 512; ++i) {
					buff[i] = *(BYTE*)(temp + i);
				}
				buff += 512;
			}
			return res;
		}

		if(SD_ReadMultiBlocks(buff, sector * 512, 512, count) != SD_OK) {
			return RES_ERROR;
		}
		if(SD_WaitReadOperation() != SD_OK) {
			return RES_ERROR;
		}
		while(SD_GetStatus() != SD_TRANSFER_OK);

		return RES_OK;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	DWORD temp[512 / 4];

	switch (pdrv) {
	case DEV_SDCARD: 
		if((uint32_t)(buff) & 0x03) {
			while(count--) {
				for(uint16_t i = 0; i < 512; ++i) {
					*(BYTE*)(temp + i) = buff[i];
				}
				
				res = disk_write(pdrv, (BYTE*)temp, sector++, 1);
				if(res != RES_OK) {
					break;
				}
				buff += 512;
			}
			return res;
		}

		if(SD_WriteMultiBlocks((uint8_t*)(buff), sector * 512, 512, count) != SD_OK) {
			return RES_ERROR;
		}
		if(SD_WaitReadOperation() != SD_OK) {
			return RES_ERROR;
		}
		while(SD_GetStatus() != SD_TRANSFER_OK);

		return RES_OK;
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
	switch (pdrv) {
	case DEV_SDCARD:
		switch(cmd) {
		case GET_SECTOR_SIZE: 
			*(WORD*)(buff) = 512;
			return RES_OK;
		case GET_BLOCK_SIZE: 
			*(DWORD*)(buff) = cardinfo.CardBlockSize;
			return RES_OK;
		case GET_SECTOR_COUNT: 
			*(DWORD*)(buff) = cardinfo.CardCapacity / cardinfo.CardBlockSize;
			return RES_OK;
		case CTRL_SYNC: 
			return RES_OK;
		}
	}

	return RES_PARERR;
}

