/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   bmp文件驱动，显示bmp图片和给液晶截图
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "./bmp/bsp_bmp.h"
#include "./usart/bsp_debug_usart.h"
#include "./FatFS/ff.h"
#include "./FatFS/ff_gen_drv.h"

/* 如果不需要打印bmp相关的提示信息,将printf注释掉即可
 * 如要用printf()，需将串口驱动文件包含进来
 */
#define BMP_DEBUG_PRINTF(FORMAT,...)    printf(FORMAT,##__VA_ARGS__)	 

/* 打印BMP文件的头信息，用于调试 */
 void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
    BMP_DEBUG_PRINTF("位图文件头:\r\n");
    BMP_DEBUG_PRINTF("文件大小:%ld\r\n",(*pBmpHead).bfSize);
    BMP_DEBUG_PRINTF("保留字:%d\r\n",(*pBmpHead).bfReserved1);
    BMP_DEBUG_PRINTF("保留字:%d\r\n",(*pBmpHead).bfReserved2);
    BMP_DEBUG_PRINTF("实际位图数据的偏移字节数:%ld\r\n",(*pBmpHead).bfOffBits);
		BMP_DEBUG_PRINTF("\r\n");	
}

/* 打印BMP文件的头信息，用于调试 */
 void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
{
    BMP_DEBUG_PRINTF("位图信息头:\r\n");
    BMP_DEBUG_PRINTF("结构体的长度:%ld\r\n",(*pBmpInforHead).biSize);
    BMP_DEBUG_PRINTF("位图宽:%ld\r\n",(*pBmpInforHead).biWidth);
    BMP_DEBUG_PRINTF("位图高:%ld\r\n",(*pBmpInforHead).biHeight);
    BMP_DEBUG_PRINTF("biPlanes平面数:%d\r\n",(*pBmpInforHead).biPlanes);
    BMP_DEBUG_PRINTF("biBitCount采用颜色位数:%d\r\n",(*pBmpInforHead).biBitCount);
    BMP_DEBUG_PRINTF("压缩方式:%ld\r\n",(*pBmpInforHead).biCompression);
    BMP_DEBUG_PRINTF("biSizeImage实际位图数据占用的字节数:%ld\r\n",(*pBmpInforHead).biSizeImage);
    BMP_DEBUG_PRINTF("X方向分辨率:%ld\r\n",(*pBmpInforHead).biXPelsPerMeter);
    BMP_DEBUG_PRINTF("Y方向分辨率:%ld\r\n",(*pBmpInforHead).biYPelsPerMeter);
    BMP_DEBUG_PRINTF("使用的颜色数:%ld\r\n",(*pBmpInforHead).biClrUsed);
    BMP_DEBUG_PRINTF("重要颜色数:%ld\r\n",(*pBmpInforHead).biClrImportant);
		BMP_DEBUG_PRINTF("\r\n");
}
