#include "main.h"
#include "quadspi.h"

#include "stm32l4xx_hal.h"
#include "Debug.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

QSPI_CommandTypeDef sCommand;
uint8_t *pData = 0;
uint32_t lineNum2InstructionMode[5] = { 
  QSPI_INSTRUCTION_NONE      , QSPI_INSTRUCTION_1_LINE,
  QSPI_INSTRUCTION_2_LINES,-1, QSPI_INSTRUCTION_4_LINES };
uint32_t lineNum2AddressMode[5] = { 
  QSPI_ADDRESS_NONE      , QSPI_ADDRESS_1_LINE,
  QSPI_ADDRESS_2_LINES,-1, QSPI_ADDRESS_4_LINES };
uint32_t lineNum2DataMode[5] = { 
  QSPI_DATA_NONE      , QSPI_DATA_1_LINE,
  QSPI_DATA_2_LINES,-1, QSPI_DATA_4_LINES };
uint32_t lineNum2AddressLen[4] = { 
  QSPI_ADDRESS_8_BITS,  QSPI_ADDRESS_16_BITS,
  QSPI_ADDRESS_24_BITS, QSPI_ADDRESS_32_BITS };
uint32_t lineNum2AlternateMode[5] = { 
  QSPI_ALTERNATE_BYTES_NONE      , QSPI_ALTERNATE_BYTES_1_LINE,
  QSPI_ALTERNATE_BYTES_2_LINES,-1, QSPI_ALTERNATE_BYTES_4_LINES };
uint32_t lineNum2AlternateLen[4] = { 
  QSPI_ALTERNATE_BYTES_8_BITS,  QSPI_ALTERNATE_BYTES_16_BITS,
  QSPI_ALTERNATE_BYTES_24_BITS, QSPI_ALTERNATE_BYTES_32_BITS };

// void QSPI_SetnLine(uint8_t lineNum){
//   if(lineNum != lineNum&0x04){nLine = 0;return;}
//   nLine = lineNum;
// }
void QSPI_SetInit(uint8_t AddrLengthinByte){
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  sCommand.AddressSize   = lineNum2AddressLen[AddrLengthinByte-1];
}
void QSPI_SetInstruct(uint8_t Cmd, uint8_t lineNum){
  sCommand.InstructionMode   = lineNum2InstructionMode[lineNum];
  sCommand.Instruction       = Cmd;
}
void QSPI_SetAddress(uint32_t Addr, uint8_t lineNum){
  sCommand.AddressMode   = lineNum2AddressMode[lineNum];
  sCommand.Address       = Addr;
}
void QSPI_SetAlternate(uint32_t AlternateByte, uint8_t len, uint8_t lineNum){
  sCommand.AlternateByteMode   = lineNum2AlternateMode[lineNum];
  sCommand.AlternateBytes      = AlternateByte;
  sCommand.AlternateBytesSize  = lineNum2AlternateLen[len];
}
void QSPI_SetDataInfo(uint8_t *Data, uint32_t len, uint32_t dummylen, uint8_t lineNum){
  sCommand.DataMode    = lineNum2DataMode[lineNum];
  sCommand.NbData      = len;
  sCommand.DummyCycles = dummylen;
  pData = Data;
}
void QSPI_Cmd(){
  HAL_QSPI_Command(&hqspi, &sCommand, 1000);
  sCommand.InstructionMode = QSPI_INSTRUCTION_NONE;
  sCommand.AddressMode = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode = QSPI_DATA_NONE;
}
void QSPI_Write(){
  QSPI_Cmd();
  HAL_QSPI_Transmit(&hqspi, pData, 1000);
}
void QSPI_Read(){
  QSPI_Cmd();
  HAL_QSPI_Receive(&hqspi, pData, 1000);
}