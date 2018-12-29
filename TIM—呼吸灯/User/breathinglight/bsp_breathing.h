#ifndef __PWM_BREATHING_H
#define	__PWM_BREATHING_H

#include "stm32f1xx.h"

extern TIM_HandleTypeDef BRE_TIM;
extern  uint16_t indexWave[];

/*PWM表中的点数*/
extern uint16_t  POINT_NUM	;


// 定时器预分频
#define BRE_TIM_PRESCALER               199

// 定时器周期
#define BRE_TIM_PERIOD                  255

/********************定时器通道**************************/
#define BRE_TIMx                        TIM3
#define BRE_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM3_CLK_ENABLE()
#define BRE_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM3_CLK_DISABLE()
#define BRE_TIM_GPIO_RCC_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE();

#define BRE_TIM_CH3_PORT                GPIOB
#define BRE_TIM_CH3_PIN                 GPIO_PIN_0

#define BRE_TIM_IRQn                    TIM3_IRQn
#define BRE_TIM_IRQHANDLER              TIM3_IRQHandler


void TIMx_Mode_Config(void);

#endif /* __PWM_BREATHING_H */
