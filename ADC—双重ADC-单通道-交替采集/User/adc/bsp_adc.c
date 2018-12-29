
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

ADC_HandleTypeDef hadcx1;
ADC_HandleTypeDef hadcx2;
DMA_HandleTypeDef hdma_adcx;


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADCx1)
  {
    /* 外设时钟使能 */
    ADCx1_RCC_CLK_ENABLE();
    
    /* AD转换通道引脚时钟使能 */
    ADC_GPIO_ClK_ENABLE();
    
    /* DMA时钟使能 */
    DMAx1_RCC_CLK_ENABLE();
      
    /* AD转换通道引脚初始化 */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);

    /* DMA外设初始化配置 */  
    hdma_adcx.Instance = ADC_DMAx_CHANNELn;
    hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcx.Init.MemInc = DMA_MINC_DISABLE;
    hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adcx.Init.Mode = DMA_CIRCULAR;
    hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adcx);
    /* 连接DMA */
    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
    
    /* 外设中断优先级配置和使能中断 */
    HAL_NVIC_SetPriority(ADC_DMAx_CHANNELn_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_DMAx_CHANNELn_IRQn);
  }
  else if(hadc->Instance==ADCx2)
  {
    /* 外设时钟使能 */
    ADCx2_RCC_CLK_ENABLE();
    
    /* AD转换通道引脚时钟使能 */
    ADC_GPIO_ClK_ENABLE();
      
    /* AD转换通道引脚初始化 */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);
  }
}

void ADCx_Init(void)
{
  ADC_MultiModeTypeDef multimode;
  ADC_ChannelConfTypeDef sConfig;

  // ADC功能配置
  hadcx1.Instance = ADCx1;
  hadcx1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadcx1.Init.ContinuousConvMode = ENABLE;
  hadcx1.Init.DiscontinuousConvMode = DISABLE;
  hadcx1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadcx1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadcx1);  
  
    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_DUALMODE_INTERLSLOW;
  HAL_ADCEx_MultiModeConfigChannel(&hadcx1, &multimode);
  
  // 配置采样通道
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx1, &sConfig);
  
  // ADC功能配置
  hadcx2.Instance = ADCx2;
  hadcx2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadcx2.Init.ContinuousConvMode = ENABLE;
  hadcx2.Init.DiscontinuousConvMode = DISABLE;
  hadcx2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadcx2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx2.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadcx2);  
  
    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_DUALMODE_INTERLSLOW;
  HAL_ADCEx_MultiModeConfigChannel(&hadcx2, &multimode);
  
  // 配置采样通道
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx2, &sConfig);
}



