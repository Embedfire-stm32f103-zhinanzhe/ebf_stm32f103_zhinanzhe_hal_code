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
#include "./led/bsp_led.h"
#include ".\key\bsp_key.h" 
#include "./rcc/bsp_clkconfig.h"

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  /* 系统时钟初始化成72MHz */
  SystemClock_Config(RCC_OSCILLATORTYPE_HSE,RCC_PLL_MUL9);
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
  
     while (1)
    {
        LED2( ON );			 // 亮 
        Delay(0x0FFFFF);
        LED2( OFF );		 // 灭
        Delay(0x0FFFFF);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
