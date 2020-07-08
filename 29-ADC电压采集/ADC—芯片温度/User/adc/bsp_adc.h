#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f1xx.h"

extern ADC_HandleTypeDef hadcx;
extern DMA_HandleTypeDef hdma_adcx;

#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC1_CLK_ENABLE()
#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC1_CLK_DISABLE()
#define DMAx_RCC_CLK_ENABLE()            __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx                             ADC1
#define ADC_DMAx_CHANNELn                DMA1_Channel1
#define ADC_CHANNEL                      ADC_CHANNEL_TEMPSENSOR 

void ADCx_Init(void);

#endif /* __BSP_ADC_H */



