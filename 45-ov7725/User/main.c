/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   液晶显示
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
#include <stdlib.h>
#include "./led/bsp_led.h" 
#include "./lcd/bsp_ili9341_lcd.h"
#include "./ov7725/bsp_ov7725.h"
#include "./systick/bsp_SysTick.h"
#include "./key/bsp_key.h"  

extern uint8_t Ov7725_vsync;
unsigned int Task_Delay[NumOfTask]; 
extern OV7725_MODE_PARAM cam_mode;



//static void LCD_Test(void);	
//static void Delay ( __IO uint32_t nCount );
void Printf_Charater(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	float frame_count = 0;
	uint8_t retry = 0;
  /* 系统时钟初始化成72MHz */
  SystemClock_Config();
  ILI9341_Init ();             //LCD 初始化
  ILI9341_GramScan( 3 );
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	/********显示字符串示例*******/
  ILI9341_DispStringLine_EN(LINE(0),"BH OV7725 Test Demo");
  /* LED 端口初始化 */
  LED_GPIO_Config();	 
  /* 初始化串口 */
  DEBUG_USART_Config();
	Key_GPIO_Config();
	SysTick_Init();

	printf("\r\n ** OV7725摄像头实时液晶显示例程** \r\n"); 
	
	/* ov7725 gpio 初始化 */
	OV7725_GPIO_Config();
	
	LED_BLUE;
	/* ov7725 寄存器默认配置初始化 */
	while(OV7725_Init() != SUCCESS)
	{
		retry++;
		if(retry>5)
		{
			printf("\r\n没有检测到OV7725摄像头\r\n");
			ILI9341_DispStringLine_EN(LINE(2),"No OV7725 module detected!");
			while(1);
		}
	}
	/*根据摄像头参数组配置模式*/
	OV7725_Special_Effect(cam_mode.effect);
	/*光照模式*/
	OV7725_Light_Mode(cam_mode.light_mode);
	/*饱和度*/
	OV7725_Color_Saturation(cam_mode.saturation);
	/*光照度*/
	OV7725_Brightness(cam_mode.brightness);
	/*对比度*/
	OV7725_Contrast(cam_mode.contrast);
	/*特殊效果*/
	OV7725_Special_Effect(cam_mode.effect);
	
	/*设置图像采样及模式大小*/
	OV7725_Window_Set(cam_mode.cam_sx,
										cam_mode.cam_sy,
										cam_mode.cam_width,
										cam_mode.cam_height,
										cam_mode.QVGA_VGA);

	/* 设置液晶扫描模式 */
	ILI9341_GramScan( cam_mode.lcd_scan );
	ILI9341_DispStringLine_EN(LINE(2),"OV7725 initialize success!");
	printf("\r\nOV7725摄像头初始化完成\r\n");
	
	Ov7725_vsync = 0;
	
	while(1)
	{
		/*接收到新图像进行显示*/
		if( Ov7725_vsync == 2 )
		{
			frame_count++;
			FIFO_PREPARE;  			/*FIFO准备*/					
			ImagDisp(cam_mode.lcd_sx,
								cam_mode.lcd_sy,
								cam_mode.cam_width,
								cam_mode.cam_height);			/*采集并显示*/
			
			Ov7725_vsync = 0;			
			LED1_TOGGLE;

		}
		
		/*检测按键*/
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )
		{
			/*LED反转*/
			LED2_TOGGLE;

		} 
		/*检测按键*/
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON  )
		{
			/*LED反转*/
			LED3_TOGGLE;			
			
		/*动态配置摄像头的模式，
			有需要可以添加使用串口、用户界面下拉选择框等方式修改这些变量，
			达到程序运行时更改摄像头模式的目的*/
	
		cam_mode.QVGA_VGA = 0,	//QVGA模式
		cam_mode.cam_sx = 0,
		cam_mode.cam_sy = 0,	

		cam_mode.cam_width = 320,
		cam_mode.cam_height = 240,

		cam_mode.lcd_sx = 0,
		cam_mode.lcd_sy = 0,
		cam_mode.lcd_scan = 3,
	//LCD扫描模式，本横屏配置可用1、3、5、7模式

		//以下可根据自己的需要调整，参数范围见结构体类型定义	
		cam_mode.light_mode = 0,//自动光照模式
		cam_mode.saturation = 0,	
		cam_mode.brightness = 0,
		cam_mode.contrast = 0,
		cam_mode.effect = 1,		//黑白模式
		
		/*根据摄像头参数写入配置*/
		OV7725_Special_Effect(cam_mode.effect);
		/*光照模式*/
		OV7725_Light_Mode(cam_mode.light_mode);
		/*饱和度*/
		OV7725_Color_Saturation(cam_mode.saturation);
		/*光照度*/
		OV7725_Brightness(cam_mode.brightness);
		/*对比度*/
		OV7725_Contrast(cam_mode.contrast);
		/*特殊效果*/
		OV7725_Special_Effect(cam_mode.effect);
		
		/*设置图像采样及模式大小*/
		OV7725_Window_Set(cam_mode.cam_sx,
											cam_mode.cam_sy,
											cam_mode.cam_width,
											cam_mode.cam_height,
											cam_mode.QVGA_VGA);

		/* 设置液晶扫描模式 */
		ILI9341_GramScan( cam_mode.lcd_scan );
		}
		
		/*每隔一段时间计算一次帧率*/
		if(Task_Delay[0] == 0)  
		{			
			printf("\r\nframe_ate = %.2f fps\r\n",frame_count/10);
			frame_count = 0;
			Task_Delay[0] = 10000;
		}
		
	}
}



/**
  * @brief  简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */	
//static void Delay ( __IO uint32_t nCount )
//{
//  for ( ; nCount != 0; nCount -- );
//	
//}

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
