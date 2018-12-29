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
/* 相关宏定义，使用存储器到存储器传输必须使用DMA1 */
DMA_HandleTypeDef DMA_Handle;

#define DMA_STREAM               DMA1_Channel6
#define DMA_STREAM_CLOCK()       __HAL_RCC_DMA1_CLK_ENABLE();
#define BUFFER_SIZE              32

/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
  const关键字将aSRC_Const_Buffer数组变量定义为常量类型 */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* 定义DMA传输目标存储器 */
uint32_t aDST_Buffer[BUFFER_SIZE];
                                                                       
static void DMA_Config(void);                                    
static void Delay(__IO uint32_t nCount);
static void SystemClock_Config(void); 
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* 定义存放比较结果变量 */
	uint8_t TransferStatus;
	/* 系统时钟初始化成216 MHz */
	SystemClock_Config();
	/* LED 端口初始化 */
	LED_GPIO_Config(); 

	/* DMA传输配置 */
	DMA_Config(); 
  
	/* 等待DMA传输完成 */
	while(__HAL_DMA_GET_FLAG(&DMA_Handle,DMA_FLAG_TC6)==RESET)
	{

	}   

	/* 比较源数据与传输后数据 */
	TransferStatus=Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);

	/* 判断源数据与传输后数据比较结果*/
	if(TransferStatus==0)  
	{
		/* 源数据与传输后数据不相等时RGB彩色灯显示红色 */
		LED_RED;
	}
	else
	{ 
		/* 源数据与传输后数据相等时RGB彩色灯显示蓝色 */
		LED_BLUE;
	}

	while (1)
	{		
	}
}

/* 简单的延时函数 */
static void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

/**
  * DMA传输配置
  */
static void DMA_Config(void)
{
	HAL_StatusTypeDef DMA_status = HAL_ERROR; 
  
  DMA_STREAM_CLOCK() ;
  
  DMA_Handle.Instance=DMA_STREAM;                            //数据流选择
                             
  DMA_Handle.Init.Direction=DMA_MEMORY_TO_MEMORY;             //存储器到外设HAL_DMA_Init(&DMA_Handle);
  DMA_Handle.Init.PeriphInc=DMA_PINC_ENABLE;                 //外设非增量模式/* Associate the DMA handle */
  DMA_Handle.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式__HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle); 
  DMA_Handle.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;    //外设数据长度:8位
  DMA_Handle.Init.MemDataAlignment=DMA_MDATAALIGN_WORD;       //存储器数据长度:8位
  DMA_Handle.Init.Mode=DMA_NORMAL;                            //外设普通模式
  DMA_Handle.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
  
  /* 完成DMA数据流参数配置 */
	HAL_DMA_Init(&DMA_Handle);

	DMA_status = HAL_DMA_Start(&DMA_Handle,(uint32_t)aSRC_Const_Buffer,(uint32_t)aDST_Buffer,BUFFER_SIZE);
  
	/* 判断DMA状态 */
	if (DMA_status != HAL_OK)
	{
		/* DMA出错就让程序运行下面循环：RGB彩色灯闪烁 */
		while (1)
		{      
			LED_RED;
			Delay(0xFFFFFF);
			LED_RGBOFF;
			Delay(0xFFFFFF);
		}
	} 
}

/**
  * 判断指定长度的两个数据源是否完全相等，
  * 如果完全相等返回1，只要其中一对数据不相等返回0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
	/* 数据长度递减 */
	while(BufferLength--)
	{
		/* 判断两个数据源是否对应相等 */
		if(*pBuffer != *pBuffer1)
		{
		  /* 对应数据源不相等马上退出函数，并返回0 */
		  return 0;
		}
		/* 递增两个数据源的地址指针 */
		pBuffer++;
		pBuffer1++;
	}
	/* 完成判断并且对应数据相对 */
	return 1;  
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
