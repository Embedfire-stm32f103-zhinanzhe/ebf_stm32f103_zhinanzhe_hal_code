/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./sdio/bsp_sdio_sdcard.h"
#include "./sdio/sdio_test.h"
#include "./led/bsp_led.h"
#include "./FatFS/ff.h"
#include "./FatFS/ff_gen_drv.h"
#include "./FatFS/drivers/sd_diskio.h"

static void printf_fatfs_error(FRESULT fresult);
/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
char SDPath[4];                   /* SD卡逻辑设备路径 */
FATFS fs;													/* FatFs文件系统对象 */
FIL file;													/* 文件对象 */
FRESULT f_res;                    /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[]= "欢迎使用野火STM32开发板 今天是个好日子，新建文件系统测试文件\r\n";


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{        
  /* 配置系统时钟为72 MHz */ 
  SystemClock_Config();
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  DEBUG_USART_Config();
  
	 /* 初始化LED灯 */
  LED_GPIO_Config();
	LED_BLUE;	
	
  /*初始化USART1*/
  DEBUG_USART_Config();
  
	/* 初始化调试串口，一般为串口1 */
	DEBUG_USART_Config();	
  printf("\r\r\n****** 这是一个SD卡 文件系统实验 ******\r\r\n");
  
	 /* 注册一个FatFS设备：SD卡 */
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    //在SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
    f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
    printf_fatfs_error(f_res);
    /*----------------------- 格式化测试 ---------------------------*/  
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if(f_res == FR_NO_FILESYSTEM)
    {
      printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
      /* 格式化 */
      f_res=f_mkfs((TCHAR const*)SDPath,0,0);							
      
      if(f_res == FR_OK)
      {
        printf("》SD卡已成功格式化文件系统。\r\n");
        /* 格式化后，先取消挂载 */
        f_res = f_mount(NULL,(TCHAR const*)SDPath,1);			
        /* 重新挂载	*/			
        f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
      }
      else
      {
        printf("《《格式化失败。》》\r\n");
        while(1);
      }
    }
    else if(f_res!=FR_OK)
    {
      printf("！！SD卡挂载文件系统失败。(%d)\r\n",f_res);
      printf_fatfs_error(f_res);
      while(1);
    }
    else
    {
      printf("》文件系统挂载成功，可以进行读写测试\r\n");
    }
    
    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("****** 即将进行文件写入测试... ******\r\n");	
    f_res = f_open(&file, "FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( f_res == FR_OK )
    {
      printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
      /* 将指定存储区内容写入到文件内 */
      f_res=f_write(&file,WriteBuffer,sizeof(WriteBuffer),&fnum);
      if(f_res==FR_OK)
      {
        printf("》文件写入成功，写入字节数据：%d\r\n",fnum);
        printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
      }
      else
      {
        printf("！！文件写入失败：(%d)\r\n",f_res);
      }    
      /* 不再读写，关闭文件 */
      f_close(&file);
    }
    else
    {	
      printf("！！打开/创建文件失败。\r\n");
    }
    
    /*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    f_res = f_open(&file, "FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ); 	 
    if(f_res == FR_OK)
    {
      printf("》打开文件成功。\r\n");
      f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum); 
      if(f_res==FR_OK)
      {
        printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
        printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
      }
      else
      {
        printf("！！文件读取失败：(%d)\r\n",f_res);
      }		
    }
    else
    {
      printf("！！打开文件失败。\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&file);
    
    /* 不再使用，取消挂载 */
    f_res = f_mount(NULL,(TCHAR const*)SDPath,1);	
  } 
  /* 注销一个FatFS设备：SD卡 */
  FATFS_UnLinkDriver(SDPath);
  
  /* 无限循环 */
  while (1)
  {
  }
}

/**
  * @brief  打印输出信息
  * @param  无
  * @retval 无
  */
static void printf_fatfs_error(FRESULT fresult)
{
  switch(fresult)
  {
    case FR_OK:                   
      printf("》操作成功。\r\n");
    break;
    case FR_DISK_ERR:             
      printf("！！硬件输入输出驱动出错。\r\n");
    break;
    case FR_INT_ERR:              
      printf("！！断言错误。\r\n");
    break;
    case FR_NOT_READY:            
      printf("！！物理设备无法工作。\r\n");
    break;
    case FR_NO_FILE:              
      printf("！！无法找到文件。\r\n");
    break;
    case FR_NO_PATH:             
      printf("！！无法找到路径。\r\n");
    break;
    case FR_INVALID_NAME:         
      printf("！！无效的路径名。\r\n");
    break;
    case FR_DENIED:               
    case FR_EXIST:                
      printf("！！拒绝访问。\r\n");
    break;
    case FR_INVALID_OBJECT:       
      printf("！！无效的文件或路径。\r\n");
    break;
    case FR_WRITE_PROTECTED:      
      printf("！！逻辑设备写保护。\r\n");
    break;
    case FR_INVALID_DRIVE:        
      printf("！！无效的逻辑设备。\r\n");
    break;
    case FR_NOT_ENABLED:          
      printf("！！无效的工作区。\r\n");
    break;
    case FR_NO_FILESYSTEM:        
      printf("！！无效的文件系统。\r\n");
    break;
    case FR_MKFS_ABORTED:         
      printf("！！因函数参数问题导致f_mkfs函数操作失败。\r\n");
    break;
    case FR_TIMEOUT:              
      printf("！！操作超时。\r\n");
    break;
    case FR_LOCKED:               
      printf("！！文件被保护。\r\n");
    break;
    case FR_NOT_ENOUGH_CORE:      
      printf("！！长文件名支持获取堆空间失败。\r\n");
    break;
    case FR_TOO_MANY_OPEN_FILES:  
      printf("！！打开太多文件。\r\n");
    break;
    case FR_INVALID_PARAMETER:    
      printf("！！参数无效。\r\n");
    break;
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV2;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
