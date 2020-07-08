
/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ADC
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "./adc/bsp_adc.h"

ADC_HandleTypeDef hadcx;
DMA_HandleTypeDef hdma_adcx;

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
   /* 外设时钟使能 */
    ADCx_RCC_CLK_ENABLE();
    
    /* DMA时钟使能 */
    DMAx_RCC_CLK_ENABLE();

    /* DMA外设初始化配置 */  
    hdma_adcx.Instance = ADC_DMAx_CHANNELn;
    hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adcx.Init.Mode = DMA_CIRCULAR;
    hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adcx);
    /* 连接DMA */
    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
}

void ADCx_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

  // ADC功能配置
  hadcx.Instance = ADCx;
  hadcx.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadcx.Init.ContinuousConvMode = ENABLE;
  hadcx.Init.DiscontinuousConvMode = DISABLE;
  hadcx.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadcx.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadcx);

  // 配置采样通道
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx, &sConfig);
}



