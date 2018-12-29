#ifndef __TOUCHPAD_H_
#define	__TOUCHPAD_H_

#include "stm32f1xx.h"
#define TOUCHPAD_GATE_VAL 	70
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（TOUCHPAD_TIMx_PRESCALER+1）
#define TOUCHPAD_TIM_PRESCALER              47 //1.5MHz
// 定义定时器周期
#define TOUCHPAD_TIM_ARR                     0xFFFF


/******************** TPAD 引脚配置参数定义 **************************/
#define TOUCHPAD_TIMx                        TIM5
#define TOUCHPAD_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM5_CLK_ENABLE()
#define TOUCHPAD_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM5_CLK_DISABLE()

#define TOUCHPAD_GPIO_RCC_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TOUCHPAD_GPIO_PIN                    GPIO_PIN_1
#define TOUCHPAD_GPIO                        GPIOA
#define TOUCHPAD_TIM_CHANNEL                 TIM_CHANNEL_2
#define TOUCHPAD_TIM_FLAG_CCR                TIM_FLAG_CC2

/************************** TPAD 函数声明********************************/
uint8_t TPAD_Init(void);
uint8_t TPAD_Scan(uint8_t mode);
 

#endif /* __TOUCHPAD_H_ */

