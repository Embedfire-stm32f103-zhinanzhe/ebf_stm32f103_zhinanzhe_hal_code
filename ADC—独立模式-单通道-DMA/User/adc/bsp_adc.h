#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f1xx.h"

// ADC GPIO 宏定义
#define RHEOSTAT_ADC_GPIO_PORT              GPIOC
#define RHEOSTAT_ADC_GPIO_PIN               GPIO_PIN_1
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
    
// ADC 序号宏定义
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __HAL_RCC_ADC1_CLK_ENABLE(); 
#define RHEOSTAT_ADC_CHANNEL                ADC_CHANNEL_11

// ADC DMA 通道宏定义，这里我们使用DMA传输
#define RHEOSTAT_ADC_DMA_CLK_ENABLE()       __HAL_RCC_DMA1_CLK_ENABLE();
#define RHEOSTAT_ADC_DMA_STREAM             DMA1_Channel1

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



