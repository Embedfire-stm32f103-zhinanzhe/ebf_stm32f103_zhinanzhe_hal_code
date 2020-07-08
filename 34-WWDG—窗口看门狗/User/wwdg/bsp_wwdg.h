#ifndef __BSP_WWDG_H
#define	__BSP_WWDG_H


#include "stm32f1xx.h"
// 窗口计数器值，范围为：0x40~0x7f，一般设置成最大0X7F

void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv);
void WWDG_Feed(void);

extern WWDG_HandleTypeDef WWDG_Handle;

#endif /* __BSP_WWDG_H */
