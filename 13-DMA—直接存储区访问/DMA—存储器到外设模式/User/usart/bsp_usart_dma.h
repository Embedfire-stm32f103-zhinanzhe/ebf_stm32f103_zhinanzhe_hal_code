#ifndef __USART_DMA_H
#define	__USART_DMA_H

#include "stm32f1xx.h"
#include <stdio.h>

//串口波特率
#define DEBUG_USART_BAUDRATE                    115200
//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE();
#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10


#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9


#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		USART1_IRQn
/************************************************************/
//DMA
#define SENDBUFF_SIZE                     		1000//发送的数据量
#define DEBUG_USART_DMA_CLK_ENABLE()      		__HAL_RCC_DMA1_CLK_ENABLE();	
#define DEBUG_USART_DMA_STREAM            		DMA1_Channel4

void Debug_USART_Config(void);
void USART_DMA_Config(void);

#endif /* __USART1_H */
