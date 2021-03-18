#include <stdio.h>

#ifndef __IS25_H__
#define __IS25_H__

void IS25LP128_Init();
void IS25LP128_Read(uint32_t Addr, uint8_t *Data, uint32_t len);
void IS25LP128_SectorErase(uint32_t Addr);
void IS25LP128_Write(uint32_t Addr, uint8_t *Data, uint32_t len);
void IS25LP128_Sleep();
void IS25LP128_Wakeup();

#endif