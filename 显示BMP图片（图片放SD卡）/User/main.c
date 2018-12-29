/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./sdio/bsp_sdio_sdcard.h"
#include "./sdio/sdio_test.h"
#include "./led/bsp_led.h"
#include "./FatFS/ff.h"
#include "./FatFS/ff_gen_drv.h"
#include "./FatFS/drivers/sd_diskio.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./font/fonts.h"	
#include "./bmp/bsp_bmp.h"
#include <stdio.h>
#include ".\key\bsp_key.h" 

 void printf_fatfs_error(FRESULT fresult);
/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
char SDPath[4];                   /* SD卡逻辑设备路径 */
FATFS fs;													/* FatFs文件系统对象 */
FIL file,bmpfdst;									/* 文件对象 */
FRESULT f_res;                    /* 文件操作结果 */

BYTE pColorData[960];			/* 一行真彩色数据缓存 320 * 3 = 960 */
#define BMP_DEBUG_PRINTF(FORMAT,...)    printf(FORMAT,##__VA_ARGS__)	 
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{ 
  int i, j, k;
	int width, height, l_width;

	BYTE red,green,blue;
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	WORD fileType;

	unsigned int read_num;
  
  /* 配置系统时钟为72 MHz */ 
  SystemClock_Config();
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  DEBUG_USART_Config();
  
	 /* 初始化LED灯 */
  LED_GPIO_Config();

	Key_GPIO_Config();
  
	ILI9341_Init ();         //LCD 初始化
	
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  ILI9341_GramScan ( 6 );

	/* 初始化调试串口，一般为串口1 */
	DEBUG_USART_Config();	
  
  
  printf("\r\n ********** 显示BMP文件及截图*********** \r\n"); 
	printf("\r\n 实验前请确保SD卡的目录下具有实验使用的BMP图片文件, \r\n"); 
	printf("\r\n 按下板子的KEY1按键可以生成屏幕截图 \r\n"); 
	 /* 注册一个FatFS设备：SD卡 */
  FATFS_LinkDriver(&SD_Driver, SDPath); 

  //在SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
  f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
  printf_fatfs_error(f_res);
  
  ILI9341_GramScan ( 6 );
  
  f_res = f_open( &file , "wildfire_1.bmp", FA_OPEN_EXISTING | FA_READ);	
  
  printf_fatfs_error(f_res);

 /*-------------------------------------------------------------------------------------------------------*/
	if(f_res == FR_OK)
	{
		BMP_DEBUG_PRINTF("打开文件成功\r\n");

		/* 读取文件头信息  两个字节*/         
		f_read(&file,&fileType,sizeof(WORD),&read_num);     

		/* 判断是不是bmp文件 "BM"*/
		if(fileType != 0x4d42)
		{
			BMP_DEBUG_PRINTF("这不是一个 .bmp 文件!\r\n");
//			return;
		}
		else
		{
			BMP_DEBUG_PRINTF("这是一个 .bmp 文件\r\n");	
		}        

		/* 读取BMP文件头信息*/
		f_read(&file,&bitHead,sizeof(tagBITMAPFILEHEADER),&read_num);        
		showBmpHead(&bitHead);

		/* 读取位图信息头信息 */
		f_read(&file,&bitInfoHead,sizeof(BITMAPINFOHEADER),&read_num);        
		showBmpInforHead(&bitInfoHead);
	}    
	else
	{
		BMP_DEBUG_PRINTF("打开文件失败!错误代码：f_res = %d \r\n",f_res);
	}    
/*-------------------------------------------------------------------------------------------------------*/
	width = bitInfoHead.biWidth;
	height = bitInfoHead.biHeight;

	/* 计算位图的实际宽度并确保它为32的倍数	*/
	l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);	

	if(l_width > 960)
	{
		BMP_DEBUG_PRINTF("\n 本图片太大，无法在液晶屏上显示 (<=320)\n");
	}
	
	
	/* 开一个图片大小的窗口*/
	ILI9341_OpenWindow(0, 0, width, height);
	ILI9341_Write_Cmd (CMD_SetPixel ); 

	
	/* 判断是否是24bit真彩色图 */
	if( bitInfoHead.biBitCount >= 24 )
	{
		for ( i = 0; i < height; i ++ )
		{
			/*从文件的后面读起，BMP文件的原始图像方向为右下角到左上角*/
      f_lseek ( & file, bitHead .bfOffBits + ( height - i - 1 ) * l_width );	
			
			/* 读取一行bmp的数据到数组pColorData里面 */
			f_read ( & file, pColorData, l_width, & read_num );				

			for(j=0; j<width; j++) 											   //一行有效信息
			{
				k = j*3;																	 //一行中第K个像素的起点
				red = pColorData[k+2];
				green = pColorData[k+1];
				blue = 	pColorData[k];
				ILI9341_Write_Data ( RGB24TORGB16 ( red, green, blue ) ); //写入LCD-GRAM
			}            			
		}        		
	}    	
	else 
	{        
		BMP_DEBUG_PRINTF("这不是一个24位真彩色BMP文件！");
	}
   /* 不再使用，取消挂载 */
   f_mount(NULL,(TCHAR const*)SDPath,1);	
}

  
  
/**
  * @brief  打印输出信息
  * @param  无
  * @retval 无
  */
 void printf_fatfs_error(FRESULT fresult)
{
  switch(fresult)
  {
    case FR_OK:                   
      printf("》操作成功。\r\n");
    break;
    case FR_DISK_ERR:             
      printf("！！硬件输入输出驱动出错。\r\n");
    break;
    case FR_INT_ERR:              
      printf("！！断言错误。\r\n");
    break;
    case FR_NOT_READY:            
      printf("！！物理设备无法工作。\r\n");
    break;
    case FR_NO_FILE:              
      printf("！！无法找到文件。\r\n");
    break;
    case FR_NO_PATH:             
      printf("！！无法找到路径。\r\n");
    break;
    case FR_INVALID_NAME:         
      printf("！！无效的路径名。\r\n");
    break;
    case FR_DENIED:               
    case FR_EXIST:                
      printf("！！拒绝访问。\r\n");
    break;
    case FR_INVALID_OBJECT:       
      printf("！！无效的文件或路径。\r\n");
    break;
    case FR_WRITE_PROTECTED:      
      printf("！！逻辑设备写保护。\r\n");
    break;
    case FR_INVALID_DRIVE:        
      printf("！！无效的逻辑设备。\r\n");
    break;
    case FR_NOT_ENABLED:          
      printf("！！无效的工作区。\r\n");
    break;
    case FR_NO_FILESYSTEM:        
      printf("！！无效的文件系统。\r\n");
    break;
    case FR_MKFS_ABORTED:         
      printf("！！因函数参数问题导致f_mkfs函数操作失败。\r\n");
    break;
    case FR_TIMEOUT:              
      printf("！！操作超时。\r\n");
    break;
    case FR_LOCKED:               
      printf("！！文件被保护。\r\n");
    break;
    case FR_NOT_ENOUGH_CORE:      
      printf("！！长文件名支持获取堆空间失败。\r\n");
    break;
    case FR_TOO_MANY_OPEN_FILES:  
      printf("！！打开太多文件。\r\n");
    break;
    case FR_INVALID_PARAMETER:    
      printf("！！参数无效。\r\n");
    break;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV2;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
