#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f1xx.h"
extern ADC_HandleTypeDef hadcx1;
extern ADC_HandleTypeDef hadcx2;
extern DMA_HandleTypeDef hdma_adcx;

#define ADCx1_RCC_CLK_ENABLE()            __HAL_RCC_ADC1_CLK_ENABLE()
#define ADCx1_RCC_CLK_DISABLE()           __HAL_RCC_ADC1_CLK_DISABLE()
#define DMAx1_RCC_CLK_ENABLE()            __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx1                             ADC1
#define ADC_DMAx_CHANNELn                 DMA1_Channel1
#define ADC_DMAx_CHANNELn_IRQn            DMA1_Channel1_IRQn
#define ADC_DMAx_CHANNELn_IRQHANDLER      DMA1_Channel1_IRQHandler

#define ADCx2_RCC_CLK_ENABLE()            __HAL_RCC_ADC2_CLK_ENABLE()
#define ADCx2_RCC_CLK_DISABLE()           __HAL_RCC_ADC2_CLK_DISABLE()
#define ADCx2                             ADC2

#define ADC_GPIO_ClK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADC_GPIO                          GPIOC
#define ADC_GPIO_PIN                      GPIO_PIN_1       
#define ADC_CHANNEL                       ADC_CHANNEL_11

void ADCx_Init(void);

#endif /* __BSP_ADC_H */



