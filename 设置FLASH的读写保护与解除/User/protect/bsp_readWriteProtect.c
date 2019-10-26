/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   内部FLASH读写保护测试范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 霸道 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./protect/bsp_readWriteProtect.h"   
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"   

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define WRITE_PROTECTION_DISABLE

#define FLASH_USER_START_ADDR       ADDR_FLASH_PAGE_32   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR         ADDR_FLASH_PAGE_48   /* End @ of user Flash area */
#define FLASH_PAGE_TO_BE_PROTECTED (OB_WRP_PAGES0TO1 | OB_WRP_PAGES2TO3)  

#define DATA_32                     ((uint32_t)0x12345678)


#if !defined(WRITE_PROTECTION_ENABLE)&&!defined(WRITE_PROTECTION_DISABLE)
#define WRITE_PROTECTION_DISABLE
#endif /* !WRITE_PROTECTION_ENABLE && !WRITE_PROTECTION_DISABLE */

#if defined(WRITE_PROTECTION_ENABLE)&&defined(WRITE_PROTECTION_DISABLE)
#error "Switches WRITE_PROTECTION_ENABLE & WRITE_PROTECTION_DISABLE cannot be enabled in the time!"
#endif /* WRITE_PROTECTION_ENABLE && WRITE_PROTECTION_DISABLE */

uint32_t Address = 0;
uint32_t PageError = 0;
__IO TestStatus MemoryProgramStatus = PASSED;
/*用于擦除过程的变量*/
 FLASH_EraseInitTypeDef EraseInitStruct;
/*用于处理选项字节的变量*/
 FLASH_OBProgramInitTypeDef OptionsBytesStruct;




/**
  * @brief  FLASH_Test,普通的写保护配置
  * @param  运行本函数后会给扇区FLASH_WRP_SECTORS进行写保护，再重复一次会进行解写保护
  * @retval None
  */
void FLASH_Test(void)
{
	printf("初始化结束，开始测试\r\n");
	  /* 初始化测试状态 */
  MemoryProgramStatus = PASSED;
  
  /* 解锁Flash以启用闪存控制寄存器访问*/ 
  HAL_FLASH_Unlock();

  /* 解锁选项字节*/
  HAL_FLASH_OB_Unlock();

  /* 获取页面写保护状态*/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

#ifdef WRITE_PROTECTION_DISABLE
  /* 检查所需页面是否已被写保护*/
  if((OptionsBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != FLASH_PAGE_TO_BE_PROTECTED)
  {
		printf("页面已被写保护，正在恢复写保护页面\r\n");
    /*恢复写保护页面 */
    OptionsBytesStruct.OptionType   = OPTIONBYTE_WRP;
    OptionsBytesStruct.WRPState     = OB_WRPSTATE_DISABLE;
    OptionsBytesStruct.WRPPage = FLASH_PAGE_TO_BE_PROTECTED;
    if(HAL_FLASHEx_OBProgram(&OptionsBytesStruct) != HAL_OK)
    {
      while (1)
      {
       LED1_ON
       printf("运行失败\r\n");
      }
    }

    /* 生成系统重置以加载新选项字节值*/
    HAL_FLASH_OB_Launch();
  }
#elif defined WRITE_PROTECTION_ENABLE
  /* 检查所需页面是否尚未写保护*/
  if(((~OptionsBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED )!= FLASH_PAGE_TO_BE_PROTECTED)
  {
	  printf("页面未被写保护，正在启用写保护页面\r\n");
    /* 启用页面写保护*/
    OptionsBytesStruct.OptionType = OPTIONBYTE_WRP;
    OptionsBytesStruct.WRPState   = OB_WRPSTATE_ENABLE;
    OptionsBytesStruct.WRPPage    = FLASH_PAGE_TO_BE_PROTECTED;
    if(HAL_FLASHEx_OBProgram(&OptionsBytesStruct) != HAL_OK)
    {
      while (1)
      {
        LED1_ON
        printf("运行失败\r\n");
      }
    }

    /* 生成系统重置以加载新选项字节值*/
    HAL_FLASH_OB_Launch();
  }
#endif /* WRITE_PROTECTION_DISABLE */

  /*锁定选项字节*/
  HAL_FLASH_OB_Lock();

  /* 所选页面未被写保护*/
  if ((OptionsBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != 0x00)
  {
		printf("页面未被写保护，正在擦除页面\r\n");
    /*填写EraseInit结构*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
    EraseInitStruct.NbPages     = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR)/FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
      /* 
       页面擦除时发生错误。 用户可以在这里添加一些代码来处理这个错误。 PageError将包含有问题的页面，然后知道此页面上的代码错误，
       用户可以调用函数'HAL_FLASH_GetError（）'
      */
      while (1)
      {
        LED1_ON
        printf("运行失败\r\n");
      }
    }

    /*由FLASH_USER_START_ADDR和FLASH_USER_END_ADDR定义的地址处的DATA_32 FLASH字程序 */
    Address = FLASH_USER_START_ADDR;
    while (Address < FLASH_USER_END_ADDR)
    {
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
      {
        Address = Address + 4;
      }
      else
      {
        while (1)
        {
          LED1_ON
           printf("运行失败\r\n");
        }
      }
    }

    /*检查书面数据的正确性*/
    Address = FLASH_USER_START_ADDR;

    while (Address < FLASH_USER_END_ADDR)
    {
      if((*(__IO uint32_t*) Address) != DATA_32)
      {
        MemoryProgramStatus = FAILED;
      }
      Address += 4;
    }
  }
  else
  { 
    /*所需页面受写保护 */ 
    /* 检查是否允许在此页面中写入 */
    Address = FLASH_USER_START_ADDR;
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) != HAL_OK)
    {
      /* 编程期间返回错误。 */
      /* 检查WRPERR标志是否设置良好 */
      if (HAL_FLASH_GetError() == HAL_FLASH_ERROR_WRP) 
      {
        MemoryProgramStatus = FAILED;
      }
      else
      {
        while (1)
        {
          LED1_ON;
          
          printf("运行失败\r\n");
        }
      }
    }
    else
    {
      while (1)
      {
        LED1_ON
        printf("运行失败\r\n");
      }
    }
  }
  HAL_FLASH_Lock();

  /*检查程序数据是否存在问题*/
  if (MemoryProgramStatus == PASSED)
  {
    LED2_ON
		printf("检查 --> 程序数据正常\r\n");
  }
  else
  {
    while (1)
    {
      LED1_ON
      printf("运行失败\r\n");
    }
  }
	printf("测试结束\r\n");

  while (1)
  {
  }
}






