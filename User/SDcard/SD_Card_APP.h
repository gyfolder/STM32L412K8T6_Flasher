/*****************************************************************************
* | File        :   EPD_SDCard.h (SD_Card_APP.h)
* | Author      :   Waveshare Team -> gy
* | Function    :   Show SDcard BMP picto e-paper -> SD Functions
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* | This version:   V1.0 -> modified by gy
* | Date        :   2018-01-11 -> 2020-03-08
* | Info        :   Basic version -> SD Interface Only
*
******************************************************************************/
#ifndef __SD_CARD_APP_H__
#define __SD_CARD_APP_H__

#include "DEV_Config.h"

void SDCard_Init(void);
uint32_t SDCard_BMP_SD2IS25(const char *BmpName, uint32_t Addr);
uint32_t SDCard_bin(const char *FileName);
#endif