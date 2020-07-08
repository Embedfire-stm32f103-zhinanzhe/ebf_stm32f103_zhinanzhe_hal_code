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
#include "./flash/bsp_spi_flash.h" 
void Delay(__IO uint32_t nCount);
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

   
/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];

//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{   
  /* 设定系统时钟为72MHz */
  SystemClock_Config();
	
	LED_GPIO_Config();
	LED_BLUE;
	
	/* 配置串口1为：115200 8-N-1 */
	DEBUG_USART_Config();
  
	printf("\r\n这是一个16M串行flash(W25Q64)实验(SPI驱动) \r\n");
	
  /* 16M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
		/* 获取 Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();
	
	Delay( 200 );
	
	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();
	
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID) 
	{	
		printf("\r\n检测到SPI FLASH W25Q64 !\r\n");
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		printf("\r\n写入的数据为：\r\n%s", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n读出的数据为：\r\n%s", Rx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			LED_GREEN;
			printf("\r\n16M串行flash(W25Q64)测试成功!\n\r");
		}
		else
		{        
			LED_RED;
			printf("\r\n16M串行flash(W25Q64)测试失败!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		LED_RED;
		printf("\r\n获取不到 W25Q64 ID!\n\r");
	}
	
	SPI_Flash_PowerDown();  
	while(1);  
}

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

void Delay(__IO uint32_t nCount)
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
