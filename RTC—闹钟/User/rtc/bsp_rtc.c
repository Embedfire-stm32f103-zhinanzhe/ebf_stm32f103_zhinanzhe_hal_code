/**
  ******************************************************************************
  * @file    bsp_rtc.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   stm32 RTC 驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "./usart/bsp_debug_usart.h"
#include "./rtc/bsp_rtc.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./beep/bsp_beep.h"  

__IO uint8_t Alarmflag =0;

RTC_HandleTypeDef Rtc_Handle;
/**
  * @brief  设置时间和日期
  * @param  无
  * @retval 无
  */
void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  sdatestructure.Year = YEAR;
  sdatestructure.Month = MONTH;
  sdatestructure.Date =  DATE;
  sdatestructure.WeekDay = WEEKDAY;
  
  if(HAL_RTC_SetDate(&Rtc_Handle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    printf("\r\n初始化日期失败\r\n");
  }

  /*##-2- Configure the Time #################################################*/
  stimestructure.Hours =HOURS;
  stimestructure.Minutes = MINUTES;
  stimestructure.Seconds = SECONDS;

  if (HAL_RTC_SetTime(&Rtc_Handle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
   printf("\r\n初始化时钟失败\r\n");;
  }

  /* 在RTC备份数据Register1中写入数据 */
  HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DRX,RTC_BKP_DATA);
}

/**
  * @brief  显示时间和日期
  * @param  无
  * @retval 无
  */
void RTC_TimeAndDate_Show(void)
{
	uint8_t Rtctmp=0;
	char LCDTemp[100];
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	while(1)
	{
		// 获取日历
		HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStructure, RTC_FORMAT_BIN);
			
		// 每秒打印一次
		if(Rtctmp != RTC_TimeStructure.Seconds)
		{
							
			// 打印日期
			printf("The Date :  Y:%0.2d - M:%0.2d - D:%0.2d - W:%0.2d\r\n", 
			RTC_DateStructure.Year,
			RTC_DateStructure.Month, 
			RTC_DateStructure.Date,
			RTC_DateStructure.WeekDay);

			//液晶显示日期
			//先把要显示的数据用sprintf函数转换为字符串，然后才能用液晶显示函数显示
			sprintf(LCDTemp,"The Date :  Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d", 
			RTC_DateStructure.Year,
			RTC_DateStructure.Month, 
			RTC_DateStructure.Date,
			RTC_DateStructure.WeekDay);
		#ifdef USE_LCD_DISPLAY
			ILI9341_DispStringLine_EN(LINE(2),LCDTemp); 
#endif
			
			// 打印时间
			printf("The Time :  %0.2d:%0.2d:%0.2d \r\n\r\n", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes, 
			RTC_TimeStructure.Seconds);
			
			//液晶显示时间
			sprintf(LCDTemp,"The Time :  %0.2d:%0.2d:%0.2d", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes, 
			RTC_TimeStructure.Seconds);
#ifdef USE_LCD_DISPLAY
			ILI9341_DispStringLine_EN(LINE(5),LCDTemp); 
#endif	
       if(Alarmflag)
      { 
         BEEP_ON;
         Alarmflag = 0;       
      }
		}
		Rtctmp = RTC_TimeStructure.Seconds;
	}	
}

/**
  * @brief  RTC配置：选择RTC时钟源
  * @param  无
  * @retval 无
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{  
	RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- 启用PWR时钟并启用对备份域的访问###################################*/
  /* 要更改RTC功能（LSE，LSI）的源时钟，您必须：
     - 使用__HAL_RCC_PWR_CLK_ENABLE（）启用电源时钟
     - 在使用之前使用HAL_PWR_EnableBkUpAccess（）函数启用写访问 配置RTC时钟源（复位后执行一次）。
     - 使用__HAL_RCC_BACKUPRESET_FORCE（）和重置备份域
     __HAL_RCC_BACKUPRESET_RELEASE（）。
     - 配置所需的RTc时钟源*/
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  
  /* 为备份寄存器启用BKP CLK */
  __HAL_RCC_BKP_CLK_ENABLE();
  
  /*##-2-将ISE配置为RTC时钟源 ###################################*/ 
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE  ;  
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON; 
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    printf("\n\r RTC 配置失败 \r\n");;
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
   printf("\n\r RTC 时钟初始化失败 \r\n");
  }
  
  /*##-3- 启用RTC外设时钟 #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

}

/**
  * @brief   要使能 RTC 闹钟中断，需按照以下顺序操作：
             1. 配置 NVIC 中的 RTC_Alarm IRQ 通道并将其使能。
             2. 配置 RTC 以生成 RTC 闹钟（闹钟 A 或闹钟 B）。
  * @param  无
  * @retval 无
  */
void RTC_AlarmSet(void)
{
    RTC_AlarmTypeDef  RTC_AlarmStructure;

    /* RTC 闹钟中断配置 */
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 0);
    /* 使能RTC闹钟中断 */
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

    /* 设置闹钟时间 */
    RTC_AlarmStructure.Alarm = RTC_Alarm_X;
  
    RTC_AlarmStructure.AlarmTime.Hours   = ALARM_HOURS;
    RTC_AlarmStructure.AlarmTime.Minutes = ALARM_MINUTES;
    RTC_AlarmStructure.AlarmTime.Seconds = ALARM_SECONDS;
  
    HAL_RTC_SetAlarm_IT(&Rtc_Handle,&RTC_AlarmStructure, RTC_FORMAT_BCD); 
}




/**********************************END OF FILE*************************************/
