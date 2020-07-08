/**
  ******************************************************************************
  * @file    bsp_usart_dma.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重现c库printf函数到usart端口,使用DMA模式发送数据
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_usart_dma.h"

uint8_t SendBuff[SENDBUFF_SIZE];

DMA_HandleTypeDef  DMA_Handle;      //DMA句柄
UART_HandleTypeDef UartHandle;      //UART句柄

 /**
  * @brief  USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
      
  DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();
  /* 使能 UART 时钟 */
  DEBUG_USART_CLK_ENABLE();
  
   /* 配置Tx引脚为复用功能  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
    GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
  
  
  UartHandle.Instance          = DEBUG_USART;
  UartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle); 
}

///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

/**
  * @brief  USART1 TX DMA 配置，内存到外设(USART1->DR)
  * @param  无
  * @retval 无
  */
void USART_DMA_Config(void)
{
  DEBUG_USART_DMA_CLK_ENABLE();  
     
  DMA_Handle.Instance=DEBUG_USART_DMA_STREAM;                            //数据流选择
                             
  DMA_Handle.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设HAL_DMA_Init(&DMA_Handle);
  DMA_Handle.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式/* Associate the DMA handle */
  DMA_Handle.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式__HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle); 
  DMA_Handle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
  DMA_Handle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
  DMA_Handle.Init.Mode=DMA_NORMAL;                            //外设普通模式
  DMA_Handle.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
  
  HAL_DMA_Init(&DMA_Handle);
  /* Associate the DMA handle */
  __HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle); 
  
}


/*********************************************END OF FILE**********************/
