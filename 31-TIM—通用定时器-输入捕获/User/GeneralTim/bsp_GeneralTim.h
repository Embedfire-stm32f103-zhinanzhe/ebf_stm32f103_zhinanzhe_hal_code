#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f1xx.h"

// 定时器预分频
#define GENERAL_TIM_PRESCALER               71

// 定时器周期
#define GENERAL_TIM_PERIOD                  0xFFFF

/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM5

#define GENERAL_TIMx                        TIM5
#define GENERAL_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM5_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM5_CLK_DISABLE()
#define GENERAL_TIM_GPIO_RCC_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE();
#define GENERAL_TIM_CHANNELx                TIM_CHANNEL_1
#define GENERAL_TIM_IT_CCx                  TIM_IT_CC1

#define GENERAL_TIM_IRQn                    TIM5_IRQn
#define GENERAL_TIM_IRQHANDLER              TIM5_IRQHandler

#define KEY1_RCC_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY1_GPIO_PIN                       GPIO_PIN_0
#define KEY1_GPIO                           GPIOA

//----------------------------------------------------------------
// 测量的起始边沿
#define            GENERAL_TIM_STRAT_ICPolarity        TIM_INPUTCHANNELPOLARITY_RISING 
// 测量的结束边沿
#define            GENERAL_TIM_END_ICPolarity          TIM_INPUTCHANNELPOLARITY_FALLING 


//----------------------------------------------------------------
// 定时器输入捕获用户自定义变量结构体声明
typedef struct              
{   
	uint8_t   ucFinishFlag;   // 捕获结束标志位
	uint8_t   ucStartFlag;    // 捕获开始标志位
	uint16_t  usCtr;          // 捕获寄存器的值
	uint16_t  usPeriod;       // 自动重装载寄存器更新标志 
}STRUCT_CAPTURE; 

//----------------------------------------------------------------
extern TIM_HandleTypeDef  TIM_TimeBase;
extern STRUCT_CAPTURE TIM_ICUserValueStructure;
//----------------------------------------------------------------

/**************************函数声明********************************/
void GENERAL_TIM_Mode_Config(void); 



#endif	/* __BSP_GENERALTIME_H */


