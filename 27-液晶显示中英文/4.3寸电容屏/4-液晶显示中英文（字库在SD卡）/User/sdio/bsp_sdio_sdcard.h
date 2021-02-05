
#ifndef __SDIO_SDCARD_H
#define __SDIO_SDCARD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"


#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef


#define MSD_OK         ((uint8_t)0x00)
#define MSD_ERROR      ((uint8_t)0x01)

 
/** 
  * @brief  SD transfer state definition  
  */     
#define  SD_TRANSFER_OK                ((uint8_t)0x00)
#define  SD_TRANSFER_BUSY              ((uint8_t)0x01)

//#define SD_DETECT_PIN                    GPIO_PIN_11
//#define SD_DETECT_GPIO_PORT              GPIOF
//#define __SD_DETECT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
//#define SD_DETECT_IRQn                   EXTI15_10_IRQn
   
#define SD_DATATIMEOUT           100000000U

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

/* DMA definitions for SD DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE            __HAL_RCC_DMA2_CLK_ENABLE
#define SD_DMAx_Tx_INSTANCE               DMA2_Channel4
#define SD_DMAx_Rx_INSTANCE               DMA2_Channel4
#define SD_DMAx_Tx_IRQn                   DMA2_Channel4_5_IRQn
#define SD_DMAx_Rx_IRQn                   DMA2_Channel4_5_IRQn
#define BSP_SD_IRQHandler                 SDIO_IRQHandler
#define SD_DMAx_Tx_IRQHandler             DMA2_Channel4_5_IRQHandler
#define SD_DMAx_Rx_IRQHandler             DMA2_Channel4_5_IRQHandler


/* Exported functions --------------------------------------------------------*/
uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ITConfig(void);
void    BSP_SD_DetectIT(void);
void    BSP_SD_DetectCallback(void);
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t BSP_SD_GetCardState(void);
void    BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);
uint8_t BSP_SD_IsDetected(void);
   
/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
void    BSP_SD_MspInit(void *Params);
void    BSP_SD_AbortCallback(void);
void    BSP_SD_WriteCpltCallback(void);
void    BSP_SD_ReadCpltCallback(void); 

#endif /* __SDCARD_H */

