/**
  ******************************************************************************
  * @file    bsp_ov7725.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   OV7725驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "./ov7725/bsp_ov7725.h"
#include "bsp_sccb.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./usart/bsp_debug_usart.h"

//摄像头初始化配置
//注意：使用这种方式初始化结构体，要在c/c++选项中选择 C99 mode
OV7725_MODE_PARAM cam_mode =
{
	
/*以下包含几组摄像头配置，可自行测试，保留一组，把其余配置注释掉即可*/
/************配置1*********横屏显示*****************************/
	
	.QVGA_VGA = 0,	//QVGA模式
	.cam_sx = 0,
	.cam_sy = 0,	
	
	.cam_width = 320,
	.cam_height = 240,
	
	.lcd_sx = 0,
	.lcd_sy = 0,
	.lcd_scan = 3, //LCD扫描模式，本横屏配置可用1、3、5、7模式
	
	//以下可根据自己的需要调整，参数范围见结构体类型定义	
	.light_mode = 0,//自动光照模式
	.saturation = 0,	
	.brightness = 0,
	.contrast = 0,
	.effect = 0,		//正常模式
	
	
/**********配置2*********竖屏显示****************************/	
/*竖屏显示需要VGA模式，同分辨率情况下，比QVGA帧率稍低*/
/*VGA模式分辨率为640*480，从中取出240*320的图像进行竖屏显示*/
/*本工程不支持超过320*240或 240*320的分辨率配置*/

//	.QVGA_VGA = 1,	//VGA模式
//	//取VGA模式居中的窗口，可根据实际需要调整
//	.cam_sx = (640-240)/2,
//	.cam_sy = (480-320)/2,	
//	
//	.cam_width = 240, 
//	.cam_height = 320, //在VGA模式下，此值才可以大于240
//	
//	.lcd_sx = 0,
//	.lcd_sy = 0,
//	.lcd_scan = 0, //LCD扫描模式，本竖屏配置可用0、2、4、6模式
//	
//	//以下可根据自己的需要调整，参数范围见结构体类型定义
//	.light_mode = 0,//自动光照模式
//	.saturation = 0,	
//	.brightness = 0,
//	.contrast = 0,
//	.effect = 0,		//正常模式
	
	
	/*******配置3************小分辨率****************************/	
	/*小于320*240分辨率的，可使用QVGA模式,设置的时候注意液晶屏边界*/
	
//	.QVGA_VGA = 0,	//QVGA模式
//	//取QVGA模式居中的窗口，可根据实际需要调整
//	.cam_sx = (320-100)/2,
//	.cam_sy = (240-150)/2,	
//	
//	.cam_width = 100, 
//	.cam_height = 150, 
//	
//	/*液晶屏的显示位置也可以根据需要调整，注意不要超过边界即可*/
//	.lcd_sx = 50,
//	.lcd_sy = 50,
//	.lcd_scan = 3, //LCD扫描模式，0-7模式都支持，注意不要超过边界即可

//	//以下可根据自己的需要调整，参数范围见结构体类型定义	
//	.light_mode = 0,//自动光照模式
//	.saturation = 0,	
//	.brightness = 0,
//	.contrast = 0,
//	.effect = 0,		//正常模式

};


typedef struct Reg
{
	uint8_t Address;			       /*寄存器地址*/
	uint8_t Value;		           /*寄存器值*/
}Reg_Info;

/* 寄存器参数配置 */
Reg_Info Sensor_Config[] =
{
	{REG_CLKRC,     0x00}, /*clock config*/
	{REG_COM7,      0x46}, /*QVGA RGB565 */
	{REG_HSTART,    0x3f},
	{REG_HSIZE,     0x50},
	{REG_VSTRT,     0x03},
	{REG_VSIZE,     0x78},
	{REG_HREF,      0x00},
	{REG_HOutSize,  0x50},
	{REG_VOutSize,  0x78},
	{REG_EXHCH,     0x00},
	

	/*DSP control*/
	{REG_TGT_B,     0x7f},
	{REG_FixGain,   0x09},
	{REG_AWB_Ctrl0, 0xe0},
	{REG_DSP_Ctrl1, 0xff},
	{REG_DSP_Ctrl2, 0x20},
	{REG_DSP_Ctrl3,	0x00},
	{REG_DSP_Ctrl4, 0x00},

	/*AGC AEC AWB*/
	{REG_COM8,		0xf0},
	{REG_COM4,		0x81}, /*Pll AEC CONFIG*/
	{REG_COM6,		0xc5},
	{REG_COM9,		0x21},
	{REG_BDBase,	0xFF},
	{REG_BDMStep,	0x01},
	{REG_AEW,		0x34},
	{REG_AEB,		0x3c},
	{REG_VPT,		0xa1},
	{REG_EXHCL,		0x00},
	{REG_AWBCtrl3,  0xaa},
	{REG_COM8,		0xff},
	{REG_AWBCtrl1,  0x5d},

	{REG_EDGE1,		0x0a},
	{REG_DNSOff,	0x01},
	{REG_EDGE2,		0x01},
	{REG_EDGE3,		0x01},

	{REG_MTX1,		0x5f},
	{REG_MTX2,		0x53},
	{REG_MTX3,		0x11},
	{REG_MTX4,		0x1a},
	{REG_MTX5,		0x3d},
	{REG_MTX6,		0x5a},
	{REG_MTX_Ctrl,  0x1e},

	{REG_BRIGHT,	0x00},
	{REG_CNST,		0x25},
	{REG_USAT,		0x65},
	{REG_VSAT,		0x65},
	{REG_UVADJ0,	0x81},
	//{REG_SDE,		  0x20},	//黑白
	{REG_SDE,		  0x06},	//彩色	调节SDE这个寄存器还可以实现其他效果
	
    /*GAMMA config*/
	{REG_GAM1,		0x0c},
	{REG_GAM2,		0x16},
	{REG_GAM3,		0x2a},
	{REG_GAM4,		0x4e},
	{REG_GAM5,		0x61},
	{REG_GAM6,		0x6f},
	{REG_GAM7,		0x7b},
	{REG_GAM8,		0x86},
	{REG_GAM9,		0x8e},
	{REG_GAM10,		0x97},
	{REG_GAM11,		0xa4},
	{REG_GAM12,		0xaf},
	{REG_GAM13,		0xc5},
	{REG_GAM14,		0xd7},
	{REG_GAM15,		0xe8},
	{REG_SLOP,		0x20},

	{REG_HUECOS,	0x80},
	{REG_HUESIN,	0x80},
	{REG_DSPAuto,	0xff},
	{REG_DM_LNL,	0x00},
	{REG_BDBase,	0x99},
	{REG_BDMStep,	0x03},
	{REG_LC_RADI,	0x00},
	{REG_LC_COEF,	0x13},
	{REG_LC_XC,		0x08},
	{REG_LC_COEFB,  0x14},
	{REG_LC_COEFR,  0x17},
	{REG_LC_CTR,	0x05},
	
	{REG_COM3,		0xd0},/*Horizontal mirror image*/

	/*night mode auto frame rate control*/
	{REG_COM5,		0xf5},	 /*在夜视环境下，自动降低帧率，保证低照度画面质量*/
	//{REG_COM5,		0x31},	/*夜视环境帧率不变*/
};

uint8_t OV7725_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);	  /*结构体数组成员数目*/

volatile uint8_t Ov7725_vsync ;	 /* 帧同步信号标志，在中断函数和main函数里面使用 */



/************************************************
 * 函数名：FIFO_GPIO_Config
 * 描述  ：FIFO GPIO配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
static void FIFO_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
		/*开启时钟*/

	  __HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	
		/*(FIFO_OE--FIFO输出使能)*/
		GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;	
		GPIO_InitStructure.Pin = OV7725_OE_GPIO_PIN;
		HAL_GPIO_Init(OV7725_OE_GPIO_PORT, &GPIO_InitStructure);
	
			/*(FIFO_WRST--FIFO写复位)*/
		GPIO_InitStructure.Pin = OV7725_WRST_GPIO_PIN;
		HAL_GPIO_Init(OV7725_WRST_GPIO_PORT, &GPIO_InitStructure);
	
			/*(FIFO_RRST--FIFO读复位) */
		GPIO_InitStructure.Pin = OV7725_RRST_GPIO_PIN;
		HAL_GPIO_Init(OV7725_RRST_GPIO_PORT, &GPIO_InitStructure);
		
		/*(FIFO_RCLK-FIFO读时钟)*/
		GPIO_InitStructure.Pin = OV7725_RCLK_GPIO_PIN;
		HAL_GPIO_Init(OV7725_RCLK_GPIO_PORT, &GPIO_InitStructure);

		/*(FIFO_WE--FIFO写使能)*/
		GPIO_InitStructure.Pin = OV7725_WE_GPIO_PIN;	
		HAL_GPIO_Init(OV7725_WE_GPIO_PORT, &GPIO_InitStructure);
	

    /*(FIFO_DATA--FIFO输出数据)*/
		GPIO_InitStructure.Pin = 	OV7725_DATA_0_GPIO_PIN | OV7725_DATA_1_GPIO_PIN |
															OV7725_DATA_2_GPIO_PIN | OV7725_DATA_3_GPIO_PIN |
															OV7725_DATA_4_GPIO_PIN | 	OV7725_DATA_5_GPIO_PIN |
															OV7725_DATA_6_GPIO_PIN | 	OV7725_DATA_7_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_NOPULL; 
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(OV7725_DATA_GPIO_PORT, &GPIO_InitStructure);
		
		
    FIFO_OE_L();	  					/*拉低使FIFO输出使能*/
    FIFO_WE_H();   						/*拉高使FIFO写允许*/
		
		
}


/************************************************
 * 函数名：VSYNC_GPIO_Config
 * 描述  ：OV7725 VSYNC中断相关配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
static void VSYNC_GPIO_Config(void)
{
		
		GPIO_InitTypeDef GPIO_InitStructure;	
		/*初始化时钟，注意中断要开AFIO*/
	  __HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();	   
    
		/*初始化引脚*/
		GPIO_InitStructure.Pin =  OV7725_VSYNC_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(OV7725_VSYNC_GPIO_PORT, &GPIO_InitStructure);
	
		HAL_NVIC_SetPriority(OV7725_VSYNC_EXTI_IRQ, 0, 3);
		HAL_NVIC_EnableIRQ(OV7725_VSYNC_EXTI_IRQ);
		
}


/************************************************
 * 函数名：OV7725_GPIO_Config
 * 描述  ：初始化控制摄像头相接的IO
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
void OV7725_GPIO_Config(void)
{
	SCCB_GPIO_Config();
	FIFO_GPIO_Config();
	VSYNC_GPIO_Config();
	
}

/************************************************
 * 函数名：Sensor_Init
 * 描述  ：Sensor初始化
 * 输入  ：无
 * 输出  ：返回1成功，返回0失败
 * 注意  ：无
 ************************************************/
ErrorStatus OV7725_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;	
	
	//DEBUG("ov7725 Register Config Start......");
	
	if( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*复位sensor */
	{
		//DEBUG("sccb write data error");		
		return ERROR ;
	}	

	if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0b ) )	 /* 读取sensor ID号*/
	{
		//DEBUG("read id faild");		
		return ERROR;
	}
	//DEBUG("Sensor ID is 0x%x", Sensor_IDCode);	
	
	if(Sensor_IDCode == OV7725_ID)
	{
		for( i = 0 ; i < OV7725_REG_NUM ; i++ )
		{
			if( 0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
			{                
				//DEBUG("write reg faild", Sensor_Config[i].Address);
				return ERROR;
			}
		}
	}
	else
	{
		return ERROR;
	}
	//DEBUG("ov7725 Register Config Success");
	
	return SUCCESS;
}



/**
  * @brief  设置光照模式
  * @param  mode :光照模式，参数范围[0~5]
			@arg 0:自动
			@arg 1:晴天
			@arg 2:多云
			@arg 3:办公室
			@arg 4:家里
			@arg 5:夜晚
  * @retval 无
  */
void OV7725_Light_Mode(uint8_t mode)
{
	switch(mode)
	{
		case 0:	//Auto，自动模式
			SCCB_WriteByte(0x13, 0xff); //AWB on 
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;
		case 1://sunny，晴天
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x5a);
			SCCB_WriteByte(0x02, 0x5c);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 2://cloudy，多云
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x58);
			SCCB_WriteByte(0x02, 0x60);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 3://office，办公室
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x84);
			SCCB_WriteByte(0x02, 0x4c);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 4://home，家里
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x96);
			SCCB_WriteByte(0x02, 0x40);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		
		case 5://night，夜晚
			SCCB_WriteByte(0x13, 0xff); //AWB on
			SCCB_WriteByte(0x0e, 0xe5);
			break;	
		
		default:
			 OV7725_DEBUG("Light Mode parameter error!"); 

			break;
	}

}			


/**
  * @brief  设置饱和度
  * @param  sat:饱和度,参数范围[-4 ~ +4]             	
  * @retval 无
  */
void OV7725_Color_Saturation(int8_t sat)
{

 	if(sat >=-4 && sat<=4)
	{	
		SCCB_WriteByte(REG_USAT, (sat+4)<<4); 
		SCCB_WriteByte(REG_VSAT, (sat+4)<<4);
	}
	else
	{
		OV7725_DEBUG("Color Saturation parameter error!");
	}
	
}			


/**
  * @brief  设置光照度
	* @param  bri:光照度，参数范围[-4~+4]
  * @retval 无
  */
void OV7725_Brightness(int8_t bri)
{
	uint8_t BRIGHT_Value,SIGN_Value;	
	
	switch(bri)
	{
		case 4:
				BRIGHT_Value = 0x48;
				SIGN_Value = 0x06;
			break;
		
		case 3:
				BRIGHT_Value = 0x38;
				SIGN_Value = 0x06;		
		break;	
		
		case 2:
				BRIGHT_Value = 0x28;
				SIGN_Value = 0x06;			
		break;	
		
		case 1:
				BRIGHT_Value = 0x18;
				SIGN_Value = 0x06;			
		break;	
		
		case 0:
				BRIGHT_Value = 0x08;
				SIGN_Value = 0x06;			
		break;	
		
		case -1:
				BRIGHT_Value = 0x08;
				SIGN_Value = 0x0e;		
		break;	
		
		case -2:
				BRIGHT_Value = 0x18;
				SIGN_Value = 0x0e;		
		break;	
		
		case -3:
				BRIGHT_Value = 0x28;
				SIGN_Value = 0x0e;		
		break;	
		
		case -4:
				BRIGHT_Value = 0x38;
				SIGN_Value = 0x0e;		
		break;	
		
		default:
			OV7725_DEBUG("Brightness parameter error!");
			break;
	}

		SCCB_WriteByte(REG_BRIGHT, BRIGHT_Value); //AWB on
		SCCB_WriteByte(REG_SIGN, SIGN_Value);
}		

/**
  * @brief  设置对比度
	* @param  cnst:对比度，参数范围[-4~+4]
  * @retval 无
  */
void OV7725_Contrast(int8_t cnst)
{
	if(cnst >= -4 && cnst <=4)
	{
		SCCB_WriteByte(REG_CNST, (0x30-(4-cnst)*4));
	}
	else
	{
		OV7725_DEBUG("Contrast parameter error!");
	}
}		


/**
  * @brief  设置特殊效果
	* @param  eff:特殊效果，参数范围[0~6]:
			@arg 0:正常
			@arg 1:黑白
			@arg 2:偏蓝
			@arg 3:复古
			@arg 4:偏红
			@arg 5:偏绿
			@arg 6:反相
  * @retval 无
  */
void OV7725_Special_Effect(uint8_t eff)
{
	switch(eff)
	{
		case 0://正常
			SCCB_WriteByte(0xa6, 0x06);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0x80);
		break;
		
		case 1://黑白
			SCCB_WriteByte(0xa6, 0x26);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0x80);
		break;	
		
		case 2://偏蓝
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0xa0);
			SCCB_WriteByte(0x61, 0x40);	
		break;	
		
		case 3://复古
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x40);
			SCCB_WriteByte(0x61, 0xa0);	
		break;	
		
		case 4://偏红
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0xc0);		
		break;	
		
		case 5://偏绿
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x60);
			SCCB_WriteByte(0x61, 0x60);		
		break;	
		
		case 6://反相
			SCCB_WriteByte(0xa6, 0x46);
		break;	
				
		default:
			OV7725_DEBUG("Special Effect error!");
			break;
	}
}		



/**
  * @brief  设置图像输出窗口（分辨率）QVGA
	* @param  sx:窗口x起始位置
	* @param  sy:窗口y起始位置
	* @param  width:窗口宽度
	* @param  height:窗口高度
	* @param QVGA_VGA:0,QVGA模式，1，VGA模式
  *
	* @note 	QVGA模式 参数要求，sx + width <= 320 ,sy+height <= 240
	* 			 	VGA模式参数要求，sx + width <= 640 ,sy+height <= 480
	*					但由于 液晶屏分辨率 和 FIFO空间 的限制，本工程不适用于超过320*240的配置
	*         使用VGA模式主要是因为OV7725无法直接交换XY方向，QVGA不方便使用竖平显示，
	*					设置成VGA模式，可以使用竖屏显示，
	*					相对QVGA模式，同样分辨率下 VGA模式 图像采样帧率稍慢
  * @retval 无
  */
void OV7725_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height,uint8_t QVGA_VGA)
{
	uint8_t reg_raw,cal_temp;

	/***********QVGA or VGA *************/
	if(QVGA_VGA == 0)
	{
		/*QVGA RGB565 */
		SCCB_WriteByte(REG_COM7,0x46); 
	}
	else
	{
			/*VGA RGB565 */
		SCCB_WriteByte(REG_COM7,0x06); 
	}

	/***************HSTART*********************/
	//读取寄存器的原内容，HStart包含偏移值，在原始偏移植的基础上加上窗口偏移	
	SCCB_ReadByte(&reg_raw,1,REG_HSTART);
	
	//sx为窗口偏移，高8位存储在HSTART，低2位在HREF
	cal_temp = (reg_raw + (sx>>2));	
	SCCB_WriteByte(REG_HSTART,cal_temp ); 
	
	/***************HSIZE*********************/
	//水平宽度，高8位存储在HSIZE，低2位存储在HREF
	SCCB_WriteByte(REG_HSIZE,width>>2);//HSIZE左移两位 
	
	
	/***************VSTART*********************/
	//读取寄存器的原内容，VStart包含偏移值，在原始偏移植的基础上加上窗口偏移	
	SCCB_ReadByte(&reg_raw,1,REG_VSTRT);	
	//sy为窗口偏移，高8位存储在HSTART，低1位在HREF
	cal_temp = (reg_raw + (sy>>1));	
	
	SCCB_WriteByte(REG_VSTRT,cal_temp);
	
	/***************VSIZE*********************/
	//垂直高度，高8位存储在VSIZE，低1位存储在HREF
	SCCB_WriteByte(REG_VSIZE,height>>1);//VSIZE左移一位
	
	/***************VSTART*********************/
	//读取寄存器的原内容	
	SCCB_ReadByte(&reg_raw,1,REG_HREF);	
	//把水平宽度的低2位、垂直高度的低1位，水平偏移的低2位，垂直偏移的低1位的配置添加到HREF
	cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2)|((sx&0x03)<<4)|((sy&0x01)<<6));	
	
	SCCB_WriteByte(REG_HREF,cal_temp);
	
	/***************HOUTSIZIE /VOUTSIZE*********************/
	SCCB_WriteByte(REG_HOutSize,width>>2);
	SCCB_WriteByte(REG_VOutSize,height>>1);
	
	//读取寄存器的原内容	
	SCCB_ReadByte(&reg_raw,1,REG_EXHCH);	
	cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2));	

	SCCB_WriteByte(REG_EXHCH,cal_temp);	
}


/**
  * @brief  设置图像输出窗口（分辨率）VGA
	* @param  sx:窗口x起始位置
	* @param  sy:窗口y起始位置
	* @param  width:窗口宽度
	* @param  height:窗口高度
	* @note 	本函数参数要求，sx + width <= 640 ,sy+height <= 480
	*					但由于 液晶屏分辨率 和 FIFO空间 的限制，本工程不适用于超过320*240的配置
	*         使用本函数主要是因为OV7725无法直接交换XY方向，QVGA不方便使用竖平显示，
	*					使用本函数设置成VGA模式，可以使用竖屏显示，
	*					相对QVGA模式，同样分辨率下 VGA模式 图像采样帧率稍慢
  * @retval 无
  */
void OV7725_Window_VGA_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	
	uint8_t reg_raw,cal_temp;

	/***********QVGA or VGA *************/
	/*VGA RGB565 */
	SCCB_WriteByte(REG_COM7,0x06); 

	/***************HSTART*********************/
	//读取寄存器的原内容，HStart包含偏移值，在原始偏移植的基础上加上窗口偏移	
	SCCB_ReadByte(&reg_raw,1,REG_HSTART);
	
	//sx为窗口偏移，高8位存储在HSTART，低2位在HREF
	cal_temp = (reg_raw + (sx>>2));	
	SCCB_WriteByte(REG_HSTART,cal_temp ); 
	
	/***************HSIZE*********************/
	//水平宽度，高8位存储在HSIZE，低2位存储在HREF
	SCCB_WriteByte(REG_HSIZE,width>>2);//HSIZE左移两位 320 
	
	
	/***************VSTART*********************/
	//读取寄存器的原内容，VStart包含偏移值，在原始偏移植的基础上加上窗口偏移	
	SCCB_ReadByte(&reg_raw,1,REG_VSTRT);	
	//sy为窗口偏移，高8位存储在HSTART，低1位在HREF
	cal_temp = (reg_raw + (sy>>1));	
	
	SCCB_WriteByte(REG_VSTRT,cal_temp);
	
	/***************VSIZE*********************/
	//垂直高度，高8位存储在VSIZE，低1位存储在HREF
	SCCB_WriteByte(REG_VSIZE,height>>1);//VSIZE左移一位 240
	
	/***************VSTART*********************/
	//读取寄存器的原内容	
	SCCB_ReadByte(&reg_raw,1,REG_HREF);	
	//把水平宽度的低2位、垂直高度的低1位，水平偏移的低2位，垂直偏移的低1位的配置添加到HREF
	cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2)|((sx&0x03)<<4)|((sy&0x01)<<6));	
	
	SCCB_WriteByte(REG_VSTRT,cal_temp);
	
	/***************HOUTSIZIE /VOUTSIZE*********************/
	SCCB_WriteByte(REG_HOutSize,width>>2);
	SCCB_WriteByte(REG_VOutSize,height>>1);
	
	//读取寄存器的原内容	
	SCCB_ReadByte(&reg_raw,1,REG_EXHCH);	
	
	cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2));	

	SCCB_WriteByte(REG_EXHCH,cal_temp);	
}

/**
  * @brief  设置显示位置
	* @param  sx:x起始显示位置
	* @param  sy:y起始显示位置
	* @param  width:显示窗口宽度,要求跟OV7725_Window_Set函数中的width一致
	* @param  height:显示窗口高度，要求跟OV7725_Window_Set函数中的height一致
  * @retval 无
  */
void ImagDisp(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t i, j; 
	uint16_t Camera_Data;
	
	ILI9341_OpenWindow(sx,sy,width,height);
	ILI9341_Write_Cmd ( CMD_SetPixel );	

	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
			ILI9341_Write_Data(Camera_Data);
		}
	}
}




/****************************End OF File*************************************/
