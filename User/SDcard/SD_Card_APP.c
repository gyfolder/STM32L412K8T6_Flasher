/*****************************************************************************
* | File        :   SD_Card_APP.cpp
* | Author      :   Waveshare team
* | Function    :   Read the BMP file in the SD card and parse it
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/
#include "SD_Card_APP.h"
#include "SD_Card_Drive.h"
#include "Debug.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "user_diskio.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "IS25.h"

//char* pDirectoryFiles[MAX_BMP_FILES];
extern char USERPath[4];   /* USER logical drive path */
extern FATFS USERFatFS;    /* File system object for USER logical drive */
extern FIL USERFile;       /* File object for USER */
FILINFO USERFileInfo;
DIR USERDirectory;

/******************************************************************************
function:	Mount SD card
parameter:
Info:
    If the initialization fails, stop the game until the
    initialization is successful
******************************************************************************/
void SDCard_Init(void)
{
    FRESULT f_res;
//    uint32_t counter = 0;

    if(FATFS_LinkDriver(&USER_Driver, USERPath) == 0) {
        /* 1.Initialize the SD mounted */
        if(BSP_SD_Init() != MSD_OK) {
            DEBUG("SD card initialization failed, check if SD card is inserted.\r\n");
            while(1);
        } else {
            DEBUG("SD card initialization successful..\r\n");
        }

        /* 2.Check the mounted device */
        f_res = f_mount(&USERFatFS, (TCHAR const*)"0:", 1);
        if(f_res != FR_OK) {
            DEBUG("SD card mount file system failed ,error code :(%d %s)\r\n",f_res, FRESULTPrinter(f_res));
            return;
        } else {
            DEBUG("SD card mount file system success!! \r\n");
            /* Initialize the Directory Files pointers (heap) */
//            for (counter = 0; counter < MAX_BMP_FILES; counter++) {
//                pDirectoryFiles[counter] = malloc(11);
//            }
        }
    } else {
        DEBUG("FATFS_LinkDriver error\r\n");
    }

    /* 3.Enter the directory */
    f_res = f_opendir(&USERDirectory, "/");
    if((f_res != FR_OK)) {
        if(f_res == FR_NO_FILESYSTEM) {
            /* Display message: SD card not FAT formated */
            DEBUG("SD card not FAT formated\r\n");
        } else {
            /* Display message: Fail to open directory */
            DEBUG("Fail to open directory\r\n");
        }
        DEBUG("SD card mount file system failed ,error code :(%d %s)\r\n",f_res, FRESULTPrinter(f_res));
        return;
    } else {
        DEBUG("file open \r\n");
    }

}
uint32_t SDCard_BMP_SD2IS25(const char *BmpName, uint32_t Addr)
{
    uint32_t IS25index = 0;
    uint32_t filesize;
    FIL bmpfile;
    UINT BytesRead;
    uint8_t Buf[257] = {0,};

    /* 1.Open file read file header */
    if(f_open(&bmpfile, BmpName, FA_READ) == FR_OK) {
        DEBUG("open bmp file: %s\r\n", BmpName);
    } else {
        DEBUG("No file found: %s\r\n", BmpName);
        return -1;
    }
    filesize = f_size(&bmpfile);
    DEBUG("File size : %d\r\n",filesize);
    for(int i=0;i<filesize;i+=256){
        f_read(&bmpfile, Buf, 256, &BytesRead);
        IS25LP128_Write(Addr + IS25index, Buf, BytesRead);
        IS25index += BytesRead;
        if(BytesRead != 256)break;
    }
    f_close(&bmpfile);
    return filesize;
}
extern uint32_t Debug_uint1;
extern uint32_t Debug_uint2;
uint32_t SDCard_bin_IS252SD(const char *ObinName, uint32_t StAddr, uint32_t Length)
{
    uint32_t IS25index = StAddr;
    uint32_t filesize = Length;
    FIL binfile;
    UINT BytesRead;
    uint8_t Buf[8193] = {0,};

    /* 1.Open file read file header */
    if(f_open(&binfile, ObinName, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
        DEBUG("Create bin file: %s\r\n", ObinName);
    } else {
        DEBUG("Update bin file: %s\r\n", ObinName);
        return -1;
    }
    //filesize = f_size(&binfile);
    DEBUG("Destined File size : %d byte(s)\r\n",filesize);


    for(int i=0;i<filesize;i+=8192){
        IS25LP128_Read(IS25index, Buf, 8192);
        Debug_uint2 = 1;
        f_write(&binfile, Buf, 8192, &BytesRead);
        Debug_uint2 = 2;

        IS25index += BytesRead;
        if(BytesRead != 8192)break;
        Debug_uint1 = IS25index;
    }
    DEBUG("IS25index : %d\r\n",IS25index);
    f_close(&binfile);
    return filesize;
}
uint32_t SDCard_bin_SD2IS25(const char *FileName, uint32_t Addr)
{
    return 0;
    uint32_t IS25index = 0;
    uint32_t filesize;
    FIL binfile;
    UINT BytesRead;
    uint8_t Buf[257] = {0,};

    /* 1.Open file read file header */
    if(f_open(&binfile, FileName, FA_READ) == FR_OK) {
        DEBUG("open file: %s\r\n", FileName);
    } else {
        DEBUG("No file found: %s\r\n", FileName);
        return -1;
    }
    filesize = f_size(&binfile);
    DEBUG("File size : %d\r\n",filesize);
    for(int i=0;i<filesize;i+=256){
        f_read(&binfile, Buf, 256, &BytesRead);
        IS25LP128_Write(Addr + IS25index, Buf, BytesRead);
        IS25index += BytesRead;
        if(BytesRead != 256)break;
    }
    f_close(&binfile);
    return filesize;
}


uint32_t SDCard_bin(const char *FileName)
{
    DEBUG("-------------------------------------------------------\r\n");
    DEBUG("SDCard_bin: %s\r\n", FileName);
    DEBUG("-------------------------------------------------------\r\n");
    uint32_t filesize;
    FIL binfile;
    UINT BytesRead;
    uint8_t Buf[4096] = {0,};
    uint8_t Verify[4096] = {0,};

    /* 1.Open file read file header */
    if(f_open(&binfile, FileName, FA_READ) == FR_OK) {
        DEBUG("open file: %s\r\n", FileName);
    } else {
        DEBUG("No file found: %s\r\n", FileName);
        return -1;
    }

    uint32_t IS25index = 0;
    filesize = f_size(&binfile);
    DEBUG("File size : %d\r\n",filesize);


    // int rtn = SDCard_bin_IS252SD("InData.bin", 0, filesize);
    // DEBUG("rtn = SDCard_bin_IS252SD() : %d\r\n",rtn);

    
    
    // DEBUG("IS25LP128_Read - 0x100 bytes\r\n");
    // IS25LP128_Read(0, Buf, 0x100);
    // PrintMemoryMap(0, Buf, 0x100, 16);

    // DEBUG("SDCard_Read - 0x100 bytes\r\n");
    // f_read(&binfile, Buf, 0x100, &BytesRead);
    // PrintMemoryMap(0, Buf, BytesRead, 16);

    // f_close(&binfile);
    // DEBUG("-------------------------------------------------------\r\n");
    // return 0;

    //filesize = 0x8000;
    int err=0, rtn = 0;
    for(int i=0;i<filesize;i+=0x1000){
        f_read(&binfile, Buf, 0x1000, &BytesRead);

        IS25LP128_SectorErase(IS25index);

        IS25LP128_Write(IS25index, Buf, BytesRead);
        
        IS25LP128_Read(IS25index, Verify, BytesRead);
        
        // DEBUG("-----------Buf-----------\r\n");
        // PrintMemoryMap(IS25index, Buf, BytesRead, 32);

        // DEBUG("-----------Verify-----------\r\n");
        // PrintMemoryMap(IS25index, Verify, BytesRead, 32);
        IS25index += BytesRead;
        rtn = memcmp(Buf, Verify, BytesRead);
        err += rtn;
        DEBUG("%04d.[%07d/%07d %02d.%02d%%] 0x%08X (%d) memcmp = %d [%d]\r\n",i>>12 ,i ,filesize ,(i*100)/filesize, ((i*10000)/filesize)%100 ,IS25index, BytesRead, rtn, err);

        if(BytesRead != 0x1000 || rtn)break;
    }
    f_close(&binfile);
    return filesize;
}