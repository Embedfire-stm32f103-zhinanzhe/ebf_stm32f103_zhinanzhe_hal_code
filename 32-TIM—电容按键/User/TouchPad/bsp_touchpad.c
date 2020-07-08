/**
  ******************************************************************************
  * @file    bsp_touchpad.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   电容按键应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./TouchPad/bsp_touchpad.h"
#include "./usart/bsp_debug_usart.h"
#include "stm32f1xx_hal.h"

//定时器最大计数值
#define TPAD_ARR_MAX_VAL 	0XFFFF	

//保存没按下时定时器计数值
__IO uint16_t tpad_default_val=0;

/***********************************
 *
 * 定时器输入捕获配置
 *
 ***********************************/	
 TIM_HandleTypeDef TIM_Handle;
 
 void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* 基本定时器外设时钟使能 */
  TOUCHPAD_TIM_RCC_CLK_ENABLE();
  /* 定时器通道引脚时钟使能 */
  TOUCHPAD_GPIO_RCC_CLK_ENABLE();
  /* 定时器通道引脚配置：捕获功能设置为输入模式 */
  GPIO_InitStruct.Pin = TOUCHPAD_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(TOUCHPAD_GPIO, &GPIO_InitStruct);
}
  
 
static void TPAD_TIM_Mode_Config(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;
  
  TIM_Handle.Instance = TOUCHPAD_TIMx;
  TIM_Handle.Init.Prescaler = TOUCHPAD_TIM_PRESCALER;
  TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TIM_Handle.Init.Period = TOUCHPAD_TIM_ARR;
  TIM_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&TIM_Handle);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&TIM_Handle, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&TIM_Handle, &sMasterConfig);
  
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 3;
  HAL_TIM_IC_ConfigChannel(&TIM_Handle, &sConfigIC, TOUCHPAD_TIM_CHANNEL);
}

/****************************************
 *
 * 为电容按键放电
 * 清除定时器标志及计数
 *
 *****************************************/
static void TPAD_Reset(void)
{
     /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* 使能电容按键引脚对应IO端口时钟 */  
  TOUCHPAD_GPIO_RCC_CLK_ENABLE();
  
  /* 设置引脚输出为低电平 */
  HAL_GPIO_WritePin(TOUCHPAD_GPIO, TOUCHPAD_GPIO_PIN, GPIO_PIN_RESET);  
  
  /* 设定电容按键对应引脚IO编号 */
  GPIO_InitStruct.Pin = TOUCHPAD_GPIO_PIN;
  /* 设定电容按键对应引脚IO为输出模式 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* 设定电容按键对应引脚IO操作速度 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* 初始化电容按键对应引脚IO */
  HAL_GPIO_Init(TOUCHPAD_GPIO, &GPIO_InitStruct);  

	HAL_Delay(5);

	__HAL_TIM_SET_COUNTER(&TIM_Handle,0); // 清零定时器计数  
  __HAL_TIM_CLEAR_FLAG(&TIM_Handle, TIM_FLAG_UPDATE|TIM_FLAG_CC2);//清除中断标志
  
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TOUCHPAD_GPIO, &GPIO_InitStruct);
  
  HAL_TIM_IC_Start(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);  
}

/****************************************************
 *
 * 得到定时器捕获值
 * 如果超时,则直接返回定时器的计数值.
 *
 *****************************************************/
static uint16_t TPAD_Get_Val(void)
{		
  TPAD_Reset();
	while(__HAL_TIM_GET_FLAG(&TIM_Handle,TOUCHPAD_TIM_FLAG_CCR)==RESET)
  {    
    uint16_t count;
    count=__HAL_TIM_GET_COUNTER(&TIM_Handle);
		if(count>(TOUCHPAD_TIM_ARR-500))
      return count;//超时了,直接返回CNT的值
	};	
	return HAL_TIM_ReadCapturedValue(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);
} 	

/****************************************************
 *
 * 读取n次,取最大值
 * n：连续获取的次数
 * 返回值：n次读数里面读到的最大读数值
 * 
 *****************************************************/
static uint16_t TPAD_Get_MaxVal(uint8_t n)
{
	uint16_t temp=0;
	uint16_t res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();//得到一次值
		if(temp>res)res=temp;
	};
	return res;
}  

/********************************************************
*
* 初始化触摸按键
* 获得空载的时候触摸按键的取值.
* 返回值:0,初始化成功;1,初始化失败
*
*********************************************************/
uint8_t TPAD_Init(void)
{
	uint16_t buf[10];
	uint32_t temp=0;
	uint8_t j,i;
	
	/* 以1.5Mhz的频率计数  */
	TPAD_TIM_Mode_Config();
  HAL_TIM_IC_Start(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);
  
  /* 连续读取10次 */
	for(i=0;i<10;i++)
	{				 
		buf[i]=TPAD_Get_Val();
		HAL_Delay(10);	    
	}
  /* 排序 */
	for(i=0;i<9;i++)
	{
		for(j=i+1;j<10;j++)
		{
      /* 升序排列 */
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
  /* 取中间的6个数据进行平均 */
	for(i=2;i<8;i++)temp+=buf[i];
	tpad_default_val=temp/6;
	printf("tpad_default_val:%d\r\n",tpad_default_val);	
  /* 初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常! */
	if(tpad_default_val>TOUCHPAD_TIM_ARR/2)return 1;
	return 0;		     			   
}
 

/*******************************************************************************
*
* 扫描触摸按键
* mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按下)
* 返回值:0,没有按下;1,有按下;			
*
*******************************************************************************/
//阈值：捕获时间必须大于(tpad_default_val + TPAD_GATE_VAL),才认为是有效触摸.
#define TPAD_GATE_VAL 	100	

uint8_t TPAD_Scan(uint8_t mode)
{
	//0,可以开始检测;>0,还不能开始检测	
	static uint8_t keyen=0;
	//扫描结果
	uint8_t res=0;
	//默认采样次数为3次
	uint8_t sample=3;	
  //捕获值	
	uint16_t rval;
	
	if(mode)
	{
		//支持连按的时候，设置采样次数为6次
		sample=6;	
		//支持连按	
		keyen=0;	  
	}	
	/* 获取当前捕获值(返回 sample 次扫描的最大值) */
	rval=TPAD_Get_MaxVal(sample); 	
	/* printf打印函数调试使用，用来确定阈值TPAD_GATE_VAL，在应用工程中应注释掉 */
	printf("scan_rval=%d\n",rval);
	
	//大于tpad_default_val+TPAD_GATE_VAL,且小于10倍tpad_default_val,则有效
	if(rval>(tpad_default_val+TPAD_GATE_VAL)&&rval<(10*tpad_default_val))
	{			
    //keyen==0,有效 		
		if(keyen==0)
		{
			res=1;		 
		}			
		keyen=3;				//至少要再过3次之后才能按键有效   
	}
	
	if(keyen)
	{
		keyen--;		
	}		
	return res;
}	 
/*********************************************END OF FILE**********************/
