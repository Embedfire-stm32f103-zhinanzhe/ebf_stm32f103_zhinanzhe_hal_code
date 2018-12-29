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

__IO uint32_t CRCValue = 0;		 // 用于存放产生的CRC校验值
CRC_HandleTypeDef crc;

void CRC_Init(void)
{
  /* 外设时钟使能 */
  __HAL_RCC_CRC_CLK_ENABLE();
  crc.Instance = CRC;
  HAL_CRC_Init(&crc);
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  uint8_t i = 0;  
  /* 配置系统时钟为72 MHz */ 
  SystemClock_Config();
	/* 初始化RGB彩灯 */ 
  CRC_Init(); 
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  DEBUG_USART_Config();
  	
 /* Compute the CRC of "DataBuffer" */
	for(i=0; i<BUFFER_SIZE; i++ ) 
	{
		//对该寄存器进行写操作时，作为输入寄存器，可以输入要进行CRC计算的新数据。
		//对该寄存器进行读操作时，返回上一次CRC计算的结果。
		//每一次写入数据寄存器，其计算结果是前一次CRC计算结果和新计算结果的组合(对整个32位字
		//进行CRC计算，而不是逐字节地计算)。
		CRCValue = HAL_CRC_Accumulate(&crc,(uint32_t *)DataBuffer, BUFFER_SIZE);
		printf("\r\n32-bit CRC 校验码为:0X%X\r\n", CRCValue);
	}
	printf("\r\nCRC(循环冗余校验)测试成功\r\n");

	for(;;)
	{
		
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
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
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
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
