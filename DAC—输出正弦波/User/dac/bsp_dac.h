#ifndef __DACx_H
#define	__DACx_H


#include "stm32f1xx.h"

#define POINT_NUM 32

#define DACx                             DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define DACx_CHANNEL_GPIO_CLK_DISABLE()  __HAL_RCC_DAC_CLK_DISABLE()
#define DACx_CLK_ENABLE()                __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_FORCE_RESET()               __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()             __HAL_RCC_DAC_RELEASE_RESET()

#define DACx_CHANNEL_PIN                 GPIO_PIN_5
#define DACx_CHANNEL_GPIO_PORT           GPIOA

#define DACx_DMAx_CHANNELn               DMA2_Channel4
#define DACx_DMAx_CHANNELn_IRQn          DMA2_Channel4_5_IRQn
#define DACx_DMAx_CHANNELn_IRQHANDLER    DMA2_Channel4_5_IRQHandler
#define DACx_CHANNEL                     DAC_CHANNEL_2


void DAC_Config(void);

extern uint16_t Sine12bit[POINT_NUM]; 

extern TIM_HandleTypeDef TIM_Time;
extern DMA_HandleTypeDef DMA_InitStructure;
extern DAC_HandleTypeDef  DAC_InitStructure;
#endif /* __DACx_H */

