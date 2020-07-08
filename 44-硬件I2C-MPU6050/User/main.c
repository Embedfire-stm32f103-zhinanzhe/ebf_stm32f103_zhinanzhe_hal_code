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
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"
#include "./usart/bsp_debug_usart.h"
//#include "./systick/bsp_SysTick.h"
#include "./mpu6050/mpu6050.h"
#include "./led/bsp_led.h"  
#include "./i2c/bsp_i2c.h"

/*简单任务管理*/
uint32_t Task_Delay[NumOfTask]={0};

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	 
	short Acel[3];
	short Gyro[3];
	float Temp;
	 
	SystemClock_Config();
	
	/* LED 端口初始化 */
	LED_GPIO_Config();
	/* 串口通信初始化 */
	DEBUG_USART_Config();

	//I2C初始化
	I2cMaster_Init();
	//MPU6050初始化
	MPU6050_Init();
	
	//检测MPU6050
	if (MPU6050ReadID() == 1)
	{	
		while(1)
		{
			if(Task_Delay[0]==TASK_ENABLE)
			{
				LED2_TOGGLE;
				Task_Delay[0]=1000;
			}
			
			if(Task_Delay[1]==0)
			{
				MPU6050ReadAcc(Acel);
				printf("加速度：%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
				MPU6050ReadGyro(Gyro);
				printf("    陀螺仪%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2]);
				MPU6050_ReturnTemp(&Temp);
				printf("    温度%8.2f\r\n",Temp);
				
				Task_Delay[1]=500;//更新一次数据，可根据自己的需求，提高采样频率，如100ms采样一次
			}
		}
	}
	else
	{
		printf("\r\n没有检测到MPU6050传感器！\r\n");
		LED_RED; 
	
		while(1);	
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
