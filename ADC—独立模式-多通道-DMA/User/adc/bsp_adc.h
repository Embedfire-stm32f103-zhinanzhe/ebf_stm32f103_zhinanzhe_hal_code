#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f1xx.h"


#define    NOFCHANEL										 6

#define    ADC_PIN1                     GPIO_PIN_0
#define    ADC_CHANNEL1                 ADC_CHANNEL_10

#define    ADC_PIN2                      GPIO_PIN_1
#define    ADC_CHANNEL2                 ADC_CHANNEL_11

#define    ADC_PIN3                      GPIO_PIN_2
#define    ADC_CHANNEL3                 ADC_CHANNEL_12

#define    ADC_PIN4                      GPIO_PIN_3
#define    ADC_CHANNEL4                 ADC_CHANNEL_13

#define    ADC_PIN5                      GPIO_PIN_4
#define    ADC_CHANNEL5                  ADC_CHANNEL_14

#define    ADC_PIN6                      GPIO_PIN_5
#define    ADC_CHANNEL6                  ADC_CHANNEL_15


// ADC GPIO 宏定义
#define RHEOSTAT_ADC_GPIO_PORT              GPIOC
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
    
// ADC 序号宏定义
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __HAL_RCC_ADC1_CLK_ENABLE(); 

// ADC DMA 通道宏定义，这里我们使用DMA传输
#define RHEOSTAT_ADC_DMA_CLK_ENABLE()       __HAL_RCC_DMA1_CLK_ENABLE();
#define RHEOSTAT_ADC_DMA_STREAM             DMA1_Channel1

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



