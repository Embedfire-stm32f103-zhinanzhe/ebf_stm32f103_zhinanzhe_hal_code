
#include "bsp_GeneralTim.h"

// 定时器输入捕获用户自定义变量结构体定义
STRUCT_CAPTURE TIM_ICUserValueStructure = {0,0,0,0};

TIM_HandleTypeDef  TIM_TimeBase;

// 引脚与中断优先级配置
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
 
  /* 基本定时器外设时钟使能 */
  GENERAL_TIM_RCC_CLK_ENABLE();
  
  KEY1_RCC_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = KEY1_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KEY1_GPIO, &GPIO_InitStruct);
  
  /* 配置定时器中断优先级并使能 */
  HAL_NVIC_SetPriority(GENERAL_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(GENERAL_TIM_IRQn);
  
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
/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)
void GENERAL_TIM_Mode_Config(void)
{
  TIM_ClockConfigTypeDef CLKSource;
  TIM_MasterConfigTypeDef MasterConfig;
  TIM_IC_InitTypeDef ICConfig;
  
   TIM_TimeBase.Instance = GENERAL_TIMx;
  // 驱动CNT计数器的时钟 = Fck_int/(psc+1)
   TIM_TimeBase.Init.Prescaler = GENERAL_TIM_PRESCALER;
  // 计数器计数模式，设置为向上计数
   TIM_TimeBase.Init.CounterMode = TIM_COUNTERMODE_UP;
  // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
   TIM_TimeBase.Init.Period = GENERAL_TIM_PERIOD;
  // 时钟分频因子 ，配置死区时间时需要用到
   TIM_TimeBase.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(& TIM_TimeBase);

  CLKSource.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(& TIM_TimeBase, &CLKSource);

  MasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  MasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(& TIM_TimeBase, &MasterConfig);
  
  ICConfig.ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
  ICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  ICConfig.ICPrescaler = TIM_ICPSC_DIV1;
  ICConfig.ICFilter = 0;
  
  HAL_TIM_IC_ConfigChannel(& TIM_TimeBase, &ICConfig, GENERAL_TIM_CHANNELx);
}


/*********************************************END OF FILE**********************/
