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
#include "./led/bsp_led.h"  
#include "./485/bsp_485.h"
#include ".\key\bsp_key.h" 

static void Delay(__IO uint32_t nCount);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	char *pbuf;
	uint16_t len;
    
  /* 配置系统时钟为216 MHz */
  SystemClock_Config();

  /* 初始化RGB彩灯 */
  LED_GPIO_Config();

  /* 初始化USART1 配置模式为 115200 8-N-1 */
  DEBUG_USART_Config();
  /*初始化485使用的串口，使用中断模式接收*/
  _485_Config();
	
	Key_GPIO_Config();
	
	printf("\r\n 欢迎使用秉火  STM32 F103 开发板。\r\n");
  printf("\r\n 秉火F103 485通讯实验例程\r\n");
	
	printf("\r\n 实验步骤：\r\n");

	printf("\r\n 1.使用导线连接好两个485通讯设备\r\n");
	printf("\r\n 2.使用跳线帽连接好:5v --- C/4-5V,485-D --- PD5,485-R ---PD6 \r\n");
	printf("\r\n 3.若使用两个秉火开发板进行实验，给两个开发板都下载本程序即可。\r\n");
	printf("\r\n 4.准备好后，按下其中一个开发板的KEY1键，会使用485向外发送0-255的数字 \r\n");
	printf("\r\n 5.若开发板的485接收到256个字节数据，会把数据以16进制形式打印出来。 \r\n");
	
  while(1)
  {
		/*按一次按键发送一次数据*/
		if(	Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)
		{
			uint16_t i;
			
			LED_BLUE;
			
			_485_TX_EN();
			
			for(i=0;i<=0xff;i++)
			{
				_485_SendByte(i);	 //发送数据
			}
			
			/*加短暂延时，保证485发送数据完毕*/
			Delay(0xFFF);
			_485_RX_EN();
			
			LED_GREEN;
			
			printf("\r\n发送数据成功！\r\n"); //使用调试串口打印调试信息到终端

		}
		else
		{		
			LED_BLUE;
			
			pbuf = get_rebuff(&len);
			if(len>=256)
			{
				LED_GREEN;
				printf("\r\n接收到长度为%d的数据\r\n",len);	
				_485_DEBUG_ARRAY((uint8_t*)pbuf,len);
				clean_rebuff();
			}
		}
  }

}
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
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
  *            HSE PREDIV1                    = 2
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 0
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
