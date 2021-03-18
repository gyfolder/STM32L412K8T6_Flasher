#include "main.h"
#include "Myquadspi.h"

#include "stm32l4xx_hal.h"
#include "Debug.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint8_t ID_REG[3];
uint8_t PP_Buf[257] = {0,};

// ReaD Status Register
//------------------------------------------------------
//|   7   |  6   |  5  |  4  |  3  |  2  |   1  |   0  |
//------------------------------------------------------
//|  SRWD |  QE  | BP3 | BP2 | BP1 | BP0 | WEL  |  WIP |
//|-------|------|-----------------------|------|------|
//| Write | Quad |   Block Protect Bit   |Write | Busy |
//|Protect|Enable|                       |Enable|      |
//------------------------------------------------------
uint8_t IS25LP128_ReadStatus(){
  uint8_t read=0;
  QSPI_SetInstruct(0x05,1);
  QSPI_SetDataInfo(&read,1,0,1);
  QSPI_Read();
  return read;
}
void IS25LP128_WriteEnable(){
  //Write Enable
  QSPI_SetInstruct(0x06,1);
  QSPI_Cmd();
}

void IS25LP128_Sleep(){
  //Write Enable
  QSPI_SetInstruct(0xB9,1);
  QSPI_Cmd();
}
void IS25LP128_Wakeup(){
  //Write Enable
  QSPI_SetInstruct(0xAB,1);
  QSPI_Cmd();
}
void IS25LP128_Init(){
  QSPI_SetInit(3);
  // Read ID(IS25LP128`s ID should be "0x9D 0x60 0x18")
  QSPI_SetInstruct(0x9F,1);
  QSPI_SetDataInfo(ID_REG,3,0,1);
  QSPI_Read();
  if(ID_REG[0] == 0x9D && ID_REG[1] == 0x60 && ID_REG[2] == 0x18)
    DEBUG("Flash Device is IS25LP128\r\n");
  else if(ID_REG[0] == 0x9D && ID_REG[1] == 0x40 && ID_REG[2] == 0x11)
    DEBUG("Flash Device is IS25LQ010\r\n");
  else if(ID_REG[0] == 0xC8 && ID_REG[1] == 0x60 && ID_REG[2] == 0x17)
    DEBUG("Flash Device is GD25LQ64\r\n");
  else 
    DEBUG("Flash Device is unknown(0x%02X 0x%02X 0x%02X)\r\n",ID_REG[0],ID_REG[1],ID_REG[2]);
}
void IS25LP128_PageProgram(uint32_t Addr, uint8_t *Data){
  // Page Program
  IS25LP128_WriteEnable();
  QSPI_SetInstruct(0x02,1);
  QSPI_SetAddress(Addr,1);
  QSPI_SetDataInfo(Data,256,0,1);
  QSPI_Write();
  int try2 = 0;
  while(IS25LP128_ReadStatus() & 0x02)try2++;
  // DEBUG("Page Program 0x%06X (Busy try:%d)\r\n",Addr,try2);
}
void IS25LP128_Read(uint32_t Addr, uint8_t *Data, uint32_t len){
  if(len == 0)return;
  // DEBUG("IS25LP128_Read called : (0x%06X,Data,0x%02X)\r\n",Addr,len);
  // Normal Read
  QSPI_SetInstruct(0x03,1);
  QSPI_SetAddress(Addr,1);
  QSPI_SetDataInfo(Data,len,0,1);
  QSPI_Read();
  // DEBUG("{%x:%02X}",Addr,*Data);
}
void IS25LP128_SectorErase(uint32_t Addr){
  Addr = Addr & 0xfff000;
  // Sector Erase
  IS25LP128_WriteEnable();
  QSPI_SetInstruct(0x20,1);
  QSPI_SetAddress(Addr,1);
  QSPI_Write();

  int try1 = 0;
  while(IS25LP128_ReadStatus() & 0x02)try1++;
  // DEBUG("Sector Erase 0x%06X (Busy try:%d)\r\n",Addr,try1);
}
void IS25LP128_Write(uint32_t Addr, uint8_t *Data, uint32_t len){
  // DEBUG("Data.\r\n");
  // PrintMemoryMap(0, Data, len, 16);
  // DEBUG("Write 0x%06X\r\n",Addr);
  uint8_t Buf[256] = {0,};
  int index = 256 - (Addr&0xff);
  //0x302 0xFC 0xFE

  //0x00 0x100 0x100
  // DEBUG("0x%02X, 0x%02X, 0x%02X\r\n",Addr,len,index);
  if(Addr&0xff || (len < 0x100)){
    IS25LP128_Read(Addr&0xffffff00, Buf, Addr&0xff);
    // DEBUG("1.\r\n");PrintMemoryMap(0, Buf, 256, 16);
    // 0x3FE 0x02
    if(index > len){//len < 0x100
      //0x376 0x100
      //0x104 0xFC
      // DEBUG("0x%02X, 0x%02X\r\n",Addr + len,index - len);
      memcpy(Buf + (Addr&0xff), Data, len);
      // DEBUG("2.\r\n");PrintMemoryMap(0, Buf, 256, 16);
      IS25LP128_Read(Addr + len, Buf + (Addr&0xff) + len, index - len);
      // DEBUG("3.\r\n");PrintMemoryMap(0, Buf, 256, 16);
    }else {
      memcpy(Buf + (Addr&0xff), Data, index);
      // DEBUG("3.\r\n");PrintMemoryMap(0, Buf, 256, 16);
    }
    
    IS25LP128_PageProgram(Addr&0xffffff00, Buf);
    if(index >= len)
      return;
    len -= index;
    Addr += index;
  }else index = 0;
  //0x00 0x100 0x00
  // DEBUG("0x%02X, 0x%02X, 0x%02X\r\n",Addr,len,index);
  for(int i=0;i<(len&0xffffff00);i+=0x100){
    //0x00 0x00 0x00
    // DEBUG("0x%02X, 0x%02X, 0x%02X\r\n",i,Addr + i,index + i);
    IS25LP128_PageProgram(Addr + i, Data + index + i);
    // DEBUG("4.\r\n");PrintMemoryMap(Addr + i,Data + index + i,256,16);
  }
  index +=((len-1)&0xffffff00);
  Addr += ((len-1)&0xffffff00);
  len  -= ((len-1)&0xffffff00);
  // DEBUG("0x%02X, 0x%02X, 0x%02X\r\n",Addr,len,index);
  if(len&0xff){
    memcpy(Buf, Data + index, (len&0xff));
    IS25LP128_Read(Addr + len, Buf + len, 256 - (len&0xff));
    IS25LP128_PageProgram(Addr, Buf);
    // DEBUG("5.\r\n");PrintMemoryMap(Addr, Buf, 256, 16);
  }
}