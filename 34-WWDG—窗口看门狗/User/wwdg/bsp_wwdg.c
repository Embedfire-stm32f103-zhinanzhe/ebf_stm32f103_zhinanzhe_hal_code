 
#include "bsp_wwdg.h"   

WWDG_HandleTypeDef WWDG_Handle;

// WWDG 中断优先级初始化
static void WWDG_NVIC_Config(void)
{
  HAL_NVIC_SetPriority(WWDG_IRQn,0,0);
	HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

/* WWDG 配置函数
 * tr ：递减计时器的值， 取值范围为：0x7f~0x40
 * wr ：窗口值，取值范围为：0x7f~0x40
 * prv：预分频器值，取值可以是
 *      @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(36MHZ)/4096)/1
 *      @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(36mhz)/4096)/2
 *      @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(36mhz)/4096)/4
 *      @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(36mhz)/4096)/8
 */
void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{	
	// 开启 WWDG 时钟
	__HAL_RCC_WWDG_CLK_ENABLE();
	// 配置WWDG中断优先级
	WWDG_NVIC_Config();
	// 配置WWDG句柄即寄存器基地址
	WWDG_Handle.Instance = WWDG;
	// 设置预分频器值
	WWDG_Handle.Init.Prescaler = prv;
	// 设置上窗口值
	WWDG_Handle.Init.Window = wr;	
	// 设置计数器的值
	WWDG_Handle.Init.Counter = tr;
	// 使能提前唤醒中断
	WWDG_Handle.Init.EWIMode = WWDG_EWI_ENABLE;
	// 初始化WWDG
	HAL_WWDG_Init(&WWDG_Handle);	
}

// 喂狗
void WWDG_Feed(void)
{
	// 喂狗，刷新递减计数器的值，设置成最大WDG_CNT=0X7F
	HAL_WWDG_Refresh(&WWDG_Handle);
}

/*********************************************END OF FILE**********************/
