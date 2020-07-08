#ifndef __SCCB_H
#define __SCCB_H

#include "stm32f1xx.h"

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}	//设置为高电平		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}//输出低电平

#define SCL_H         digitalHi(GPIOC , GPIO_PIN_6) 
#define SCL_L         digitalLo(GPIOC , GPIO_PIN_6) 
   
#define SDA_H         digitalHi(GPIOC , GPIO_PIN_7) 
#define SDA_L         digitalLo(GPIOC , GPIO_PIN_7) 

#define SCL_read      HAL_GPIO_ReadPin(GPIOC , GPIO_PIN_6) 
#define SDA_read      HAL_GPIO_ReadPin(GPIOC , GPIO_PIN_7) 

#define ADDR_OV7725   0x42

void SCCB_GPIO_Config(void);
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t	SendByte);
int SCCB_ReadByte(uint8_t * pBuffer,   uint16_t length,   uint8_t ReadAddress);

#endif 
