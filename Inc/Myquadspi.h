#include <stdio.h>

#ifndef __MYQUADSPI_H__
#define __MYQUADSPI_H__

void QSPI_SetInit(uint8_t AddrLengthinByte);
void QSPI_SetInstruct(uint8_t Cmd, uint8_t lineNum);
void QSPI_SetAddress(uint32_t Addr, uint8_t lineNum);
void QSPI_SetAlternate(uint32_t AlternateByte, uint8_t len, uint8_t lineNum);
void QSPI_SetDataInfo(uint8_t *Data, uint32_t len, uint32_t dummylen, uint8_t lineNum);
void QSPI_Cmd();
void QSPI_Write();
void QSPI_Read();

#endif