/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F103 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"   

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{		
		
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*开启LED相关的GPIO外设时钟*/
    LED1_GPIO_CLK_ENABLE();
    LED2_GPIO_CLK_ENABLE();
    LED3_GPIO_CLK_ENABLE();
  
    /*选择要控制的GPIO引脚*/															   
    GPIO_InitStruct.Pin = LED1_PIN;	

    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*设置引脚速率为高速 */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);	

    /*选择要控制的GPIO引脚*/															   
    GPIO_InitStruct.Pin = LED2_PIN;	
    HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);	

    /*选择要控制的GPIO引脚*/															   
    GPIO_InitStruct.Pin = LED3_PIN;	
    HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);	

    /*关闭RGB灯*/
    LED_RGBOFF;
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


/*********************************************END OF FILE**********************/
