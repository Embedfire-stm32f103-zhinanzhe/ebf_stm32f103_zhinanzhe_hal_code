/**
  ******************************************************************************
  * @file    bsp_advance_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   高级控制定时器定时范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim/bsp_advance_tim.h"

TIM_HandleTypeDef TIM_TimeBaseStructure;

 /**
  * @brief  时钟与中断配置
  * @param  无
  * @retval 无
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{  
    /* 基本定时器外设时钟使能 */
    ADVANCED_TIM_RCC_CLK_ENABLE();

    /* 外设中断配置 */
    HAL_NVIC_SetPriority(ADVANCE_TIM_IRQn , 1, 0);
    HAL_NVIC_EnableIRQ(ADVANCE_TIM_IRQn );  
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */
 void TIM_Mode_Config(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  
  TIM_TimeBaseStructure.Instance = ADVANCED_TIMx;
  
   // 时钟预分频数
  TIM_TimeBaseStructure.Init.Prescaler = ADVANCED_TIM_PRESCALER;
  // 计数器计数模式，设置为向上计数
  TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
  // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
  TIM_TimeBaseStructure.Init.Period = ADVANCED_TIM_PERIOD;
  // 时钟分频因子 ，没用到不用管
  TIM_TimeBaseStructure.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  // 重复计数器的值，没用到不用管
  TIM_TimeBaseStructure.Init.RepetitionCounter = ADVANCED_TIM_REPETITIONCOUNTER;
  // 初始化定时器
  HAL_TIM_Base_Init(&TIM_TimeBaseStructure);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  
  HAL_TIM_ConfigClockSource(&TIM_TimeBaseStructure, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBaseStructure, &sMasterConfig);
  
  /* 在中断模式下启动定时器 */
  HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure);
}



/*********************************************END OF FILE**********************/
