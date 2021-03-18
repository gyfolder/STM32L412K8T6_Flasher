/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "gpio.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE {
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}

void vprint(const char *fmt, va_list argp){
    char string[200];
    if(0 < vsprintf(string,fmt,argp)) // build string
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
    }
}
void my_printf(const char *fmt, ...){ // custom printf() function
	va_list argp;
	va_start(argp, fmt);
	vprint(fmt, argp);
	va_end(argp);
}

void PrintMemoryMap(int _showaddress, void* _address, int len, int bitRow) {
	my_printf("PrintMemoryMap() start");
	char *_Buf1 = (char*)malloc(bitRow + 1);
	memset(_Buf1, 0, bitRow + 1);
	char* addr = (char*)_address;

	char* newline = "\r\n";
	my_printf("Address : 0x%08X%s", _showaddress, newline);
	int remainbyte = len % bitRow;
	int focus = 0;

	//my_printf((String)"focus      : " + focus);
	//my_printf((String)"remainbyte : " + remainbyte);
	//my_printf((String)"len        : " + len);
	//my_printf((String)"bitRow     : " + bitRow);
	//my_printf((String)"Consistence: " + len + "=" + (len-remainbyte) + "+" + remainbyte);

	for (; focus < (len - remainbyte); focus++) {
		if ((focus % bitRow) == 0) {
			my_printf("0x%08X: ", _showaddress + focus);
		}
		my_printf("%02X ", (int)addr[focus]);
		if (32 <= (uint8_t)addr[focus] && (uint8_t)addr[focus] <= 127)
			_Buf1[focus%bitRow] = addr[focus];
		else
			_Buf1[focus%bitRow] = '.';

		if ((focus % bitRow) == (bitRow - 1)) {
			my_printf(_Buf1);
      my_printf(newline);
			memset(_Buf1, 0, bitRow);
		}
	}

	if(remainbyte)
		for (int i = 0; i < bitRow; i++) {
			if ((focus % bitRow) == 0) {
				my_printf("0x%08X: ", _showaddress + focus);
			}
			if (i > (remainbyte - 1)) {
				my_printf("   ");
			}
			else {
				my_printf("%02X ", addr[focus]);
				if ('0' <= (uint8_t)addr[focus] && (uint8_t)addr[focus] <= 127)
					_Buf1[focus%bitRow] = addr[focus];
				else
					_Buf1[focus%bitRow] = '.';
			}
			if (i == bitRow - 1) {
				my_printf(_Buf1);
        my_printf(newline);
				memset(_Buf1, 0, bitRow);
			}
			focus++;
		}
	free(_Buf1);
	my_printf(newline);
	my_printf("PrintMemoryMap() end");
	my_printf("\r\n");
}
char *FRESULTStr[20] = {
	"FR_OK,(0) Succeeded"
	,"FR_DISK_ERR,(1) A hard error occurred in the low level disk I/O layer"
	,"FR_INT_ERR,(2) Assertion failed"
	,"FR_NOT_READY,(3) The physical drive cannot work"
	,"FR_NO_FILE,(4) Could not find the file"
	,"FR_NO_PATH,(5) Could not find the path"
	,"FR_INVALID_NAME,(6) The path name format is invalid"
	,"FR_DENIED,(7) Access denied due to prohibited access or directory full"
	,"FR_EXIST,(8) Access denied due to prohibited access"
	,"FR_INVALID_OBJECT,(9) The file/directory object is invalid"
	,"FR_WRITE_PROTECTED,(10) The physical drive is write protected"
	,"FR_INVALID_DRIVE,(11) The logical drive number is invalid"
	,"FR_NOT_ENABLED,(12) The volume has no work area"
	,"FR_NO_FILESYSTEM,(13) There is no valid FAT volume"
	,"FR_MKFS_ABORTED,(14) The f_mkfs() aborted due to any problem"
	,"FR_TIMEOUT,(15) Could not get a grant to access the volume within defined period"
	,"FR_LOCKED,(16) The operation is rejected according to the file sharing policy"
	,"FR_NOT_ENOUGH_CORE,(17) LFN working buffer could not be allocated"
	,"FR_TOO_MANY_OPEN_FILES,(18) Number of open files > _FS_LOCK"
	,"FR_INVALID_PARAMETER,(19) Given parameter is invalid"
};
char* FRESULTPrinter(int res){
	return FRESULTStr[res];
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
