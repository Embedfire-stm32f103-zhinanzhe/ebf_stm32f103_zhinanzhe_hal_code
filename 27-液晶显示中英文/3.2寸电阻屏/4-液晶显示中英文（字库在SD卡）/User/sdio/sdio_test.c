/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SDIO sd卡测试驱动（不含文件系统）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103-霸道 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "sdio/sdio_test.h"
#include "./led/bsp_led.h"
#include "./sdio/bsp_sdio_sdcard.h"
#include "./usart/bsp_debug_usart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_Test(void)
{ 
  uint8_t SD_state = MSD_OK;
  
  SD_state = BSP_SD_Init();
  
  if(SD_state != MSD_OK)
  {
    printf("SD Initialization : FAIL.\r\n");	
  }		
   else
   {
    printf("SD Initialization :  OK.\r\n");		

    SD_state = BSP_SD_Erase(BLOCK_START_ADDR, NUM_OF_BLOCKS);
    /* Wait until SD cards are ready to use for new operation */
    while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
    {
    }
    if(SD_state != MSD_OK)
    {
       printf("SD ERASE : FAILED.\r\n");  
    }
    else
    {
       printf("SD ERASE : OK.\r\n"); 
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD_state = BSP_SD_WriteBlocks(aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS,10);
      /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
      {
      }                               
      if(SD_state != MSD_OK)
      {
        printf("SD WRITE : FAILED.\r\n");
      }
      else
      {
        printf("SD WRITE : OK.\r\n");
        SD_state = BSP_SD_ReadBlocks(aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS,10);
        /* Wait until SD cards are ready to use for new operation */
        while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
        {
        }
        if(SD_state != MSD_OK)
        {
          printf("SD READ  : FAILED.\r\n");
        }
        else
        {
          printf("SD READ  : OK.\r\n");
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            printf("SD COMPARE  : FAILED.\r\n");
          }
          else
          {
            printf("SD COMPARE  : OK.\r\n");
          }
        }
      }
    }
  } 
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;
  
  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}


/*********************************************END OF FILE**********************/
