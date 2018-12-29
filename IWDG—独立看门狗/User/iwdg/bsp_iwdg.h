#ifndef __IWDG_H
#define	__IWDG_H

#include "stm32f1xx.h"


void IWDG_Config(uint8_t prv ,uint16_t rlv);

extern IWDG_HandleTypeDef IWDG_Handle;


#endif /* __IWDG_H */

