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
#include <stdlib.h>
#include "./led/bsp_led.h" 
#include "./adc/bsp_adc.h"

// 用于保存转换计算后的电压值  
__IO uint16_t Current_Temperature; 

// AD转换结果值
uint16_t ADC_ConvertedValue;

//对于12位的ADC，3.3V的ADC值为0xfff,温度为25度时对应的电压值为1.43V即0x6EE
#define V25  0x6EE
//斜率 每摄氏度4.3mV 对应每摄氏度0x05
#define AVG_SLOPE 0x05 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{   
  /* 配置系统时钟为72 MHz */
  SystemClock_Config();

  /* 初始化USART1 配置模式为 115200 8-N-1 */
  DEBUG_USART_Config();

  
	printf("\r\n 这是一个内部温度传感器实验 \r\n");
	printf( "\r\n Print current Temperature  \r\n");	
  
  ADCx_Init();
  HAL_ADCEx_Calibration_Start(&hadcx);
  
  /* 启动AD转换并使能DMA传输和中断 */
  HAL_ADC_Start_DMA(&hadcx,(uint32_t *)&ADC_ConvertedValue,sizeof(ADC_ConvertedValue));  
  
  /* 无限循环 */
  while (1)
  {
    HAL_Delay(1000);
    
    Current_Temperature = (V25-ADC_ConvertedValue)/AVG_SLOPE+25;	
    printf("The IC current temp = %3d ℃\n",Current_Temperature);

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
