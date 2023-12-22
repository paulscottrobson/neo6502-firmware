// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      null_storage.c
//      Author :    Paul Robson
//      Date :      22nd December 2023
//      Reviewed :  No
//      Purpose :   Null Storage driver.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include <inttypes.h>
#include "ff.h"
#include "diskio.h"

DSTATUS disk_status(BYTE pdrv) {
    return 0;
}

DSTATUS disk_initialize(BYTE pdrv) {
    (void)(pdrv);
    return 0;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    return RES_OK;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    return RES_OK;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_COUNT:
            *((DWORD *)buff) = 256;
            return RES_OK;
        case GET_SECTOR_SIZE:
            *((WORD *)buff) = 256;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *((DWORD *)buff) = 1;  // 1 sector
            return RES_OK;
        default:
            return RES_PARERR;
    }
}

// ***************************************************************************************
//
//      Date        Revision
//      ====        ========
//
// ***************************************************************************************
