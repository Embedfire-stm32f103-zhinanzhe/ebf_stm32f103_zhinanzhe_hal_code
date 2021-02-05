/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   sdram应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103 开发板  
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./sdram/bsp_sdram.h"  

static SRAM_HandleTypeDef  SRAM_Handler;
static FSMC_NORSRAM_TimingTypeDef Timing;

/**
  * @brief  Initializes SRAM MSP.
  */
 void FSMC_SRAM_MspInit(void)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  /* Enable FMC clock */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  
  /* 地址线:F0---F5,F12---F15,G0---G5,D11---D13;
     数据信号线;D0,D1,D8---D10,D14,D15,E7---E15;
     CS片选:G10;
     WE写使能:D5;
     OE读使能;D4;
     UB数据掩码;E1;
     LB数据掩码;E0;
  */
  
  /* GPIOD configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8     |\
                              GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |\
                              GPIO_PIN_14 | GPIO_PIN_15;
   
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* GPIOE configuration */  
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7     |\
                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |\
                              GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);
  
  /* GPIOF configuration */  
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);
  
  /* GPIOG configuration */  
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5 | GPIO_PIN_10;
  
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);   
}


/**
  * @brief  Initializes the SRAM device.
  * @retval SRAM status
  */
void FSMC_SRAM_Init(void)
{ 
   SRAM_Handler.Instance = FSMC_NORSRAM_DEVICE;
   SRAM_Handler.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  
  /* SRAM device configuration */  
  Timing.AddressSetupTime      = 0x00;
  Timing.AddressHoldTime       = 0x00;
  Timing.DataSetupTime         = 0x08;
  Timing.BusTurnAroundDuration = 0x00;
  Timing.CLKDivision           = 0x00;
  Timing.DataLatency           = 0x00;
  Timing.AccessMode            = FSMC_ACCESS_MODE_A;
 
  SRAM_Handler.Init.NSBank=FSMC_NORSRAM_BANK3;     					        //使用NE4
	SRAM_Handler.Init.DataAddressMux=FSMC_DATA_ADDRESS_MUX_DISABLE; 	//地址/数据线不复用
	SRAM_Handler.Init.MemoryType=FSMC_MEMORY_TYPE_SRAM;   				    //SRAM
	SRAM_Handler.Init.MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16; 	//16位数据宽度
	SRAM_Handler.Init.BurstAccessMode=FSMC_BURST_ACCESS_MODE_DISABLE; //是否使能突发访问,仅对同步突发存储器有效,此处未用到
	SRAM_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
	SRAM_Handler.Init.WaitSignalActive=FSMC_WAIT_TIMING_BEFORE_WS;   	//存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	SRAM_Handler.Init.WriteOperation=FSMC_WRITE_OPERATION_ENABLE;    	//存储器写使能
	SRAM_Handler.Init.WaitSignal=FSMC_WAIT_SIGNAL_DISABLE;           	//等待使能位,此处未用到
	SRAM_Handler.Init.ExtendedMode=FSMC_EXTENDED_MODE_DISABLE;        	//读写使用相同的时序
	SRAM_Handler.Init.AsynchronousWait=FSMC_ASYNCHRONOUS_WAIT_DISABLE;	//是否使能同步传输模式下的等待信号,此处未用到
	SRAM_Handler.Init.WriteBurst=FSMC_WRITE_BURST_DISABLE;           	  //禁止突发写
  
  /* SRAM controller initialization */
  FSMC_SRAM_MspInit();
  HAL_SRAM_Init(& SRAM_Handler, &Timing, &Timing);

}
		

/**
  * @brief  以“字”为单位向sdram写入数据 
  * @param  pBuffer: 指向数据的指针 
  * @param  uwWriteAddress: 要写入的SRAM内部地址
  * @param  uwBufferSize: 要写入数据大小
  * @retval None.
  */
void SRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwWriteAddress;
 

  /* 循环写入数据 */
  for (; uwBufferSize != 0; uwBufferSize--) 
  {
    /* 发送数据到SRAM */
    *(uint32_t *) (Bank1_SRAM3_ADDR + write_pointer) = *pBuffer++;

    /* 地址自增*/
    write_pointer += 4;
  }
    
}

/**
  * @brief  从SRAM中读取数据 
  * @param  pBuffer: 指向存储数据的buffer
  * @param  ReadAddress: 要读取数据的地十
  * @param  uwBufferSize: 要读取的数据大小
  * @retval None.
  */
void SRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwReadAddress;
  
  
  /*读取数据 */
  for(; uwBufferSize != 0x00; uwBufferSize--)
  {
   *pBuffer++ = *(__IO uint32_t *)(Bank1_SRAM3_ADDR + write_pointer );
    
   /* 地址自增*/
    write_pointer += 4;
  } 
}


/**
  * @brief  测试SRAM是否正常 
  * @param  None
  * @retval 正常返回1，异常返回0
  */
uint8_t SRAM_Test(void)
{
  /*写入数据计数器*/
  uint32_t counter=0;
  
  /* 8位的数据 */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16位的数据 */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  SRAM_INFO("正在检测SRAM，以8位、16位的方式读写sram...");


  /*按8位格式读写数据，并校验*/
  
  /* 把SRAM数据全部重置为0 ，IS62WV51216_SIZE是以8位为单位的 */
  for (counter = 0x00; counter < IS62WV51216_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank1_SRAM3_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* 向整个SRAM写入数据  8位 */
  for (counter = 0; counter < IS62WV51216_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank1_SRAM3_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* 读取 SRAM 数据并检测*/
  for(counter = 0; counter<IS62WV51216_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(Bank1_SRAM3_ADDR + counter);  //从该地址读出数据
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SRAM_ERROR("8位数据读写错误！");
      return 0;
    }
  }
	
  
  /*按16位格式读写数据，并检测*/
  
  /* 把SRAM数据全部重置为0 */
  for (counter = 0x00; counter < IS62WV51216_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank1_SRAM3_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* 向整个SRAM写入数据  16位 */
  for (counter = 0; counter < IS62WV51216_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank1_SRAM3_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* 读取 SRAM 数据并检测*/
  for(counter = 0; counter<IS62WV51216_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(Bank1_SRAM3_ADDR + 2*counter);  //从该地址读出数据
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SRAM_ERROR("16位数据读写错误！");

      return 0;
    }
  }
  
  SRAM_INFO("SRAM读写测试正常！"); 
  /*检测正常，return 1 */
  return 1;

}


/*********************************************END OF FILE**********************/

