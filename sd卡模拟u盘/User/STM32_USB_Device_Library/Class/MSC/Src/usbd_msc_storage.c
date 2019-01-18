/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @brief          : Memory management layer
  ******************************************************************************
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
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
#include "usbd_msc_storage.h"
/* USER CODE BEGIN INCLUDE */
#include "./sdio/bsp_sdio_sdcard.h"
/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_STORAGE 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_TYPES */
/* USER CODE END PRIVATE_TYPES */ 
/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_Private_Defines
  * @{
  */ 
#define STORAGE_LUN_NBR                  1  
#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */
  
/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_Private_Macros
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_MACRO */
/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_IF_Private_Variables
  * @{
  */
/* USB handler declaration */
/* Handle for USB Full Speed IP */
  USBD_HandleTypeDef  *hUsbDevice_0;
/* USER CODE BEGIN INQUIRY_DATA_FS */ 
/* USB Mass storage Standard Inquiry Data */
const int8_t  STORAGE_Inquirydata_FS[] = {//36
  
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1',                     /* Version      : 4 Bytes */
}; 
/* USER CODE END INQUIRY_DATA_FS */ 

/* USER CODE BEGIN PRIVATE_VARIABLES */
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */ 
 
/** @defgroup USBD_STORAGE_IF_Exported_Variables
  * @{
  */ 
  extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */ 
  
/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @{
  */
static int8_t STORAGE_Init_FS (uint8_t lun);
static int8_t STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS (uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS (uint8_t lun);
static int8_t STORAGE_Read_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr,uint16_t blk_len);
static int8_t STORAGE_Write_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr,uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS (void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */ 
  
USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS,
};

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : STORAGE_Init_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Init_FS (uint8_t lun)
{
  /* USER CODE BEGIN 2 */ 
  BSP_SD_Init();
  return (USBD_OK);
  /* USER CODE END 2 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_GetCapacity_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */    
  /* Get SD card Information */
   HAL_SD_GetCardInfo(&uSdHandle, &CardInfo);
  
  *block_num  = CardInfo.CardCapacity/STORAGE_BLK_SIZ-1;
  *block_size = STORAGE_BLK_SIZ;
  return (USBD_OK);
  /* USER CODE END 3 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_IsReady_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t  STORAGE_IsReady_FS (uint8_t lun)
{
  /* USER CODE BEGIN 4 */ 
  if(HAL_SD_GetCardState(&uSdHandle)==SD_TRANSFER_OK)
    return (USBD_OK);
  else
    return -1;
  /* USER CODE END 4 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_IsWriteProtected_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t  STORAGE_IsWriteProtected_FS (uint8_t lun)
{
  /* USER CODE BEGIN 5 */ 
  return (USBD_OK);
  /* USER CODE END 5 */ 
}

/**
  * @brief SD_DMAConfigRx
  * @par Function Description
  *   This function configure the DMA to receive data from the SD card
  * @retval
  *  SD_ERROR or SD_OK
  */
HAL_SD_ErrorTypedef SD_DMAConfigRx(SD_HandleTypeDef *hsd)
{
  static DMA_HandleTypeDef hdma_rx;
  HAL_StatusTypeDef status = HAL_ERROR;
  
  if(hsd->hdmarx == NULL)
  {
    /* Configure DMA Rx parameters */
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hdma_rx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

    hdma_rx.Instance = DMA2_Channel4;

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsd, hdmarx, hdma_rx);

    /* Stop any ongoing transfer and reset the state*/
    HAL_DMA_Abort(&hdma_rx);
    
    /* Deinitialize the Channel for new transfer */
    HAL_DMA_DeInit(&hdma_rx);

    /* Configure the DMA Channel */
    status = HAL_DMA_Init(&hdma_rx);
    
    /* NVIC configuration for DMA transfer complete interrupt */
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
  }
  else
  {
    status = HAL_OK;
  }
  
  return (status != HAL_OK? SD_ERROR : SD_OK);
}

/**
  * @brief SD_DMAConfigTx
  * @par Function Description
  *   This function configure the DMA to transmit data to the SD card
  * @retval
  *  SD_ERROR or SD_OK
  */
HAL_SD_ErrorTypedef SD_DMAConfigTx(SD_HandleTypeDef *hsd)
{
  static DMA_HandleTypeDef hdma_tx;
  HAL_StatusTypeDef status;
  
  if(hsd->hdmatx == NULL)
  {
    /* Configure DMA Tx parameters */
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hdma_tx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
    
    hdma_tx.Instance = DMA2_Channel4;
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hsd, hdmatx, hdma_tx);
    
    /* Stop any ongoing transfer and reset the state*/
    HAL_DMA_Abort(&hdma_tx);
    
    /* Deinitialize the Channel for new transfer */
    HAL_DMA_DeInit(&hdma_tx);
    
    /* Configure the DMA Channel */
    status = HAL_DMA_Init(&hdma_tx); 
    
    /* NVIC configuration for DMA transfer complete interrupt */
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);  
  }
  else
  {
    status = HAL_OK;
  }
  
  return (status != HAL_OK? SD_ERROR : SD_OK);
}

/*******************************************************************************
* Function Name  : STORAGE_Read_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Read_FS (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,                       
                        uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */ 
  uint8_t state = 0;
  uSdHandle.hdmatx=NULL;
  
  /* Prepare the dma channel for a read operation */
  state = ((SD_DMAConfigRx(&uSdHandle) == SD_OK) ? 0 : -1);
  
  if(state == 0)
  {
    /* Read block(s) in DMA transfer mode */
    state = ((HAL_SD_ReadBlocks_DMA(&uSdHandle,(uint32_t *)buf, blk_addr*STORAGE_BLK_SIZ, STORAGE_BLK_SIZ, blk_len) == SD_OK) ? 0 : -1);
    
    /* Wait until transfer is complete */
    if(state == 0)
    {
      state = ((HAL_SD_CheckReadOperation(&uSdHandle, (uint32_t)0xFFFFFFFF) == SD_OK) ? 0 : -1);
    }    
  }
  
  return (state);
  /* USER CODE END 6 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_Write_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Write_FS (uint8_t lun, 
                         uint8_t *buf, 
                         uint32_t blk_addr,
                         uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */ 
  uint8_t state = 0;
  
  /* Invalidate the dma rx handle*/
  uSdHandle.hdmarx = NULL;
  
  /* Prepare the dma channel for a read operation */
  state = ((SD_DMAConfigTx(&uSdHandle) == SD_OK) ? 0 : -1);
  
  if(state == 0)
  { 
    /* Write block(s) in DMA transfer mode */
    state = ((HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint32_t *)buf, blk_addr*STORAGE_BLK_SIZ, STORAGE_BLK_SIZ, blk_len) == SD_OK) ? 0 : -1);
    
    /* Wait until transfer is complete */
    if(state == 0)
    {
      state = ((HAL_SD_CheckWriteOperation(&uSdHandle, (uint32_t)0xFFFFFFFF) == SD_OK) ? 0 : -1);
    }
  }
  
  return state; 
  /* USER CODE END 7 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_GetMaxLun_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_GetMaxLun_FS (void)
{
  /* USER CODE BEGIN 8 */ 
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */ 
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */ 

/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
