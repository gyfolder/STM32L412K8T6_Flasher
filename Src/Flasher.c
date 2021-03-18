#include "main.h"
#include "spi.h"
#include "quadspi.h"
#include "stm32l4xx_hal.h"
#include "SD_Card_APP.h"
#include "Myquadspi.h"
#include "IS25.h"
#include "Debug.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h> // PRId64, PRIu64

extern SPI_HandleTypeDef hspi1;
extern QSPI_HandleTypeDef hqspi;

int LED_Mode = 0, LED_Mode_old = 0, blink_mode = 0;

uint8_t Cmd = 0, Data[9] = {0,};
volatile uint8_t ShowFlag = 0;

void Flasher_Init(){
  DEBUG("Let`s Initialize peripherals!!\r\n");
  QSPI_SetInit(3);
  IS25LP128_Wakeup();
  IS25LP128_Init();

  uint8_t rData[0x21] = {0,};
  IS25LP128_Read(0, rData, 0x20);
  PrintMemoryMap(0, rData, 0x20, 16);
  if(Card_Detact_GPIO_Port->IDR & Card_Detact_Pin){
    my_printf("please insert SD Card!!\r\n");
    LED_Mode = 3;
    while(Start_Btn_GPIO_Port->IDR & Start_Btn_Pin)HAL_Delay(100);
    
  }else{
    SDCard_Init();
    /*
    IS25LP128_SectorErase(0x000000);
    IS25LP128_SectorErase(0x001000);
    IS25LP128_SectorErase(0x002000);
    IS25LP128_SectorErase(0x003000);
    IS25LP128_SectorErase(0x004000);
    IS25LP128_SectorErase(0x005000);*/
    SDCard_bin("bios.bin");
    // memset(rData, 0, 0x1000);
    // IS25LP128_Read(0, rData, 0x1000);
    // PrintMemoryMap(0, rData, 0x1000, 16);

  }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM1){
    if(LED_Mode != LED_Mode_old){LED_Mode_old = LED_Mode; blink_mode = 0;}
      switch(LED_Mode){
        case 0://Not Change
          if(blink_mode == 0){
            Stat_LED_GPIO_Port->BRR = Stat_LED_Pin;
            blink_mode++;
          }
        break;
        case 1://Blinking(Duty : 10%)
          blink_mode++;
          if(blink_mode == 1)Stat_LED_GPIO_Port->BSRR = Stat_LED_Pin;
          else if(blink_mode == 2)Stat_LED_GPIO_Port->BRR = Stat_LED_Pin;
          else if(blink_mode == 10)blink_mode = 0;
        break;
        case 2://On
          if(blink_mode == 0){
            Stat_LED_GPIO_Port->BSRR = Stat_LED_Pin;
            blink_mode++;
          }
        break;
        case 3://Error(Toggling)(Duty : 50%)
          blink_mode++;
          if(blink_mode == 1)Stat_LED_GPIO_Port->BSRR = Stat_LED_Pin;
          else if(blink_mode == 6)Stat_LED_GPIO_Port->BRR = Stat_LED_Pin;
          else if(blink_mode == 10)blink_mode = 0;
        break;
      }
  }
}
void Flasher_Loop(){
}