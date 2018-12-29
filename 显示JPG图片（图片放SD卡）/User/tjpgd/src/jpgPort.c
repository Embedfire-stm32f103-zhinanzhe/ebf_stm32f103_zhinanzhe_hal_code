
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stdint.h"

#include "stm32f1xx.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./tjpgd/src/tjpgd.h"
#include "./FatFS/ff.h"
#include "./FatFS/ff_gen_drv.h"
#include "./FatFS/drivers/sd_diskio.h"


char SDPath[4];                   /* SD卡逻辑设备路径 */
FATFS fs;													/* FatFs文件系统对象 */
FIL file;									        /* 文件对象 */
FRESULT f_res;                    /* 文件操作结果 */

void printf_fatfs_error(FRESULT FRESULT);

/* User defined device identifier */
typedef struct {
    FIL *fp;      /* File pointer for input function */
    BYTE *fbuf;    /* Pointer to the frame buffer for output function */
    UINT wfbuf;    /* Width of the frame buffer [pix] */
} IODEV;


/* tiny jpg解码库接口，in_func和out_func接口， 由用户实现*/

/*从SD卡读取图片数据流，如果要从SRAM里读取数据流，则修改此函数*/
UINT in_func (JDEC* jd, BYTE* buff, UINT nbyte)
{
		UINT rwb;
		FRESULT res;
	
    IODEV *dev = (IODEV*)jd->device;   /* Device identifier for the session (5th argument of jd_prepare function) */


    if (buff) {
        /* Read bytes from input stream */
			  res = f_read(dev->fp,buff,nbyte,&rwb);
				if(res == FR_OK)
					return rwb;
				
    } else {
        /* Remove bytes from input stream */
			return (f_lseek(dev->fp, f_tell(dev->fp)+nbyte) == FR_OK) ? nbyte:0;
    }	
		
}



/*输出解码后的数据流*/
UINT out_func (JDEC* jd, void* bitmap, JRECT* rect)
{
    //IODEV *dev = (IODEV*)jd->device;
    BYTE *src;
    UINT x,y;


    /* Put progress indicator */
    if (rect->left == 0) {
        printf("\r%lu%%", (rect->top << jd->scale) * 100UL / jd->height);
    }
    
		src = (BYTE*)bitmap;	//解码后的图像数据
			
		/* 开窗 解码矩阵大小的窗口 */
		ILI9341_OpenWindow(rect->left,rect->top,(rect->right - rect->left)+1,(rect->bottom - rect->top)+1);	
    ILI9341_Write_Cmd ( CMD_SetPixel );		
		for (y = rect->top; y <= rect->bottom; y++) 
		{	
			//显示一个单元的一行，解码库以（8*8~16*16或更多）的矩阵为一个解码单元
		for(x=0;x<(rect->right - rect->left)+1;x++)						
			{				
				ILI9341_Write_Data(*(WORD*)src);
				src += 2;	
			}				
		}

    return 1;    /* Continue to decompress */
}

/* 显示图片的例子 
 * pic_name:jpg文件的完全路径		
 */
void jpgDisplay()
{
  void *work;       /* Pointer to the decompressor work area */
  JDEC jdec;        /* Decompression object */
  JRESULT res;      /* Result code of TJpgDec API */
  IODEV devid;      /* User defined device identifier */
	
  /* Open a JPEG file */
  /* 注册一个FatFS设备：SD卡 */
  FATFS_LinkDriver(&SD_Driver, SDPath); 

  //在SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
  f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
  printf_fatfs_error(f_res);
  
  ILI9341_GramScan ( 6 );
	/*显示SD卡内的JPG文件，请确保SD卡该路径下有该文件，
	可在工程目录下的“需要拷贝到SD卡”的文件夹中复制到SD卡的该目录*/
   f_res = f_open(&file,"one_piece.jpg",FA_READ|FA_OPEN_EXISTING);	
   printf_fatfs_error(f_res);     
   if(f_res == FR_OK)
   {
     devid.fp = &file; 
  }
		
		/* 申请解码空间 */
    /* Allocate a work area for TJpgDec */
    work = (char *)malloc(sizeof(char)*7*1024);
		
    /* Prepare to decompress */
    res = jd_prepare(&jdec, in_func, work, 7*1024, &devid);
    if (res == JDR_OK) {
        /* Ready to dcompress. Image info is available here. */
        printf("Image dimensions: %u by %u. %u bytes used.\n", jdec.width, jdec.height, 7*1024 - jdec.sz_pool);

				/*配置第三个参数可以设置缩放*/
        res = jd_decomp(&jdec, out_func, 0);   /* 解码并显示图片 。Start to decompress with 1/1 scaling */
       
				if (res == JDR_OK) {
	
            printf("\rOK  \n");				

        } else {
            printf("Failed to decompress: rc=%d\n", res);
        }

    } else {
        printf("Failed to prepare: rc=%d\n", res);
    }

    free(work);             /* Discard work area */

    /* 不再使用，取消挂载 */
    f_mount(NULL,(TCHAR const*)SDPath,1);	    
}


/**
  * @brief  打印输出信息
  * @param  无
  * @retval 无
  */
 void printf_fatfs_error(FRESULT fresult)
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

