/**
  ******************************************************************************
  * @file    GPIO/GPIO_EXTI/Src/stm32f3xx_it.c 
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
#include "bsp_GeneralTim.h"
#include "./led/bsp_led.h"  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
}
  
/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f7xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */	
void GENERAL_TIM_IRQHANDLER(void)
{ 
  HAL_TIM_IRQHandler(&TIM_TimeBase); 
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  TIM_ICUserValueStructure .usPeriod ++;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  TIM_IC_InitTypeDef IC_Config;
  
  if ( TIM_ICUserValueStructure .ucStartFlag == 0 )
  {
    LED1_TOGGLE;       
    
    __HAL_TIM_SET_COUNTER(htim,0); // 清零定时器计数
    TIM_ICUserValueStructure .usPeriod = 0;			
    TIM_ICUserValueStructure .usCtr = 0;
    
    // 配置输入捕获参数，主要是修改触发电平
    IC_Config.ICPolarity = GENERAL_TIM_END_ICPolarity;
    IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
    IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
    IC_Config.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&TIM_TimeBase, &IC_Config, GENERAL_TIM_CHANNELx);
    // 清除中断标志位
    __HAL_TIM_CLEAR_IT(htim, GENERAL_TIM_IT_CCx);
    // 启动输入捕获并开启中断
    HAL_TIM_IC_Start_IT(&TIM_TimeBase,GENERAL_TIM_CHANNELx);    
    TIM_ICUserValueStructure .ucStartFlag = 1;			
  }		
  
  else
  {
    LED1_TOGGLE;
    
    // 获取定时器计数值
    TIM_ICUserValueStructure .usCtr = HAL_TIM_ReadCapturedValue(&TIM_TimeBase,GENERAL_TIM_CHANNELx);
    // 配置输入捕获参数，主要是修改触发电平
    IC_Config.ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
    IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
    IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
    IC_Config.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&TIM_TimeBase, &IC_Config, GENERAL_TIM_CHANNELx);
    
    // 清除中断标志位
    __HAL_TIM_CLEAR_IT(htim, GENERAL_TIM_IT_CCx); 
    // 启动输入捕获并开启中断
    HAL_TIM_IC_Start_IT(&TIM_TimeBase,GENERAL_TIM_CHANNELx);    
    TIM_ICUserValueStructure .ucStartFlag = 0;			
    TIM_ICUserValueStructure .ucFinishFlag = 1;    
  }
}



/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
