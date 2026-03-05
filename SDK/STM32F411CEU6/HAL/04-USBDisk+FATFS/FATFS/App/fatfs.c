/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
#include "w25qxx.h"
#include <stdio.h>
#include <string.h>
uint8_t workbuf[4096];
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */   
	char wtext[100];
  uint32_t byteswritten;                     /* File write/read counts */
	if(retUSER == FR_OK)
	{
		retUSER = f_mount(&USERFatFS,(TCHAR const*)USERPath,1) ;
		if(retUSER != FR_OK)
		{
			retUSER = 2;
			f_mkfs((TCHAR const*)USERPath, 0, 0,workbuf,4096);		
		}
		else
		{
			/*
			FA_READ ָ�������ʶ��󡣿��Դ��ļ��ж�ȡ���ݡ���FA_WRITE��Ͽ��Խ��ж�д���ʡ�
			FA_WRITE ָ��д���ʶ��󡣿������ļ���д�����ݡ���FA_READ��Ͽ��Խ��ж�д���ʡ�
			FA_OPEN_EXISTING ���ļ�������ļ������ڣ����ʧ�ܡ�(Ĭ��)
			FA_OPEN_ALWAYS ����ļ����ڣ���򿪣����򣬴���һ�����ļ���
			FA_CREATE_NEW ����һ�����ļ�������ļ��Ѵ��ڣ��򴴽�ʧ�ܡ�
			FA_CREATE_ALWAYS ����һ�����ļ�������ļ��Ѵ��ڣ����������ضϲ����ǡ�
			ע�⣺�� _FS_READONLY == 1 ʱ��ģʽ��־ FA_WRITE, FA_CREATE_ALWAYS, FA_CREATE_NEW, FA_OPEN_ALWAYS ����Ч�ġ�
			*/
			if(f_open(&USERFile, "WeAct Studio Core Board.txt", FA_CREATE_NEW | FA_WRITE) == FR_OK)
			{
				#define Flash_Size (0x1FFF7A22UL)
				
				sprintf(wtext,"MCU ID: 0x%x, ROM: %dKB\r\n\r\n",HAL_GetDEVID(),(*(__IO uint16_t*)(Flash_Size)));
				f_write(&USERFile, wtext, strlen(wtext), (void *)&byteswritten);
				
				sprintf(wtext,"x25QXX\r\nFlash ID: 0X%X\r\nFlash Size: %d MB\r\n\r\n",W25Qx_Para.FLASH_Id,W25Qx_Para.FLASH_Size / 1024 / 1024);
				f_write(&USERFile, wtext, strlen(wtext), (void *)&byteswritten);
				
				sprintf(wtext,"This is STM32 working with FatFs\r\nSTM32F4x1 Core Board Design By WeAct Studio\r\nBoard Version: 3.1\r\n");
				f_write(&USERFile, wtext, strlen(wtext), (void *)&byteswritten);
				
				f_close(&USERFile);
			}
		}
		
		retUSER = f_mount(&USERFatFS,"",1) ;
	}   	  
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return ((2020 - 1980) << 25) /* Year */
	| (6 << 21) /* Month */
	| (1 << 16) /* Day */
	| (12 << 11) /* Hour */
	| (0 << 5) /* Min */
	| (0) /* Sec */
	;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */
