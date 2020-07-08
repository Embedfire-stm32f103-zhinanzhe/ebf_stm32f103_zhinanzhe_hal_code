#ifndef __OV7725_H
#define __OV7725_H 
	   
#include "stm32f1xx.h"


/*摄像头配置结构体*/
typedef struct
{	

	
	uint8_t QVGA_VGA; //0：QVGA，1VGA
	
	/*VGA:sx + width <= 320 或 240 ,sy+height <= 320 或 240*/
	/*QVGA:sx + width <= 320 ,sy+height <= 240*/
	uint16_t cam_sx; //摄像头窗口X起始位置
	uint16_t cam_sy; //摄像头窗口Y起始位置
	
	uint16_t cam_width;//图像分辨率，宽
	uint16_t cam_height;//图像分辨率，高
	
	uint16_t lcd_sx;//图像显示在液晶屏的X起始位置
	uint16_t lcd_sy;//图像显示在液晶屏的Y起始位置
	uint8_t lcd_scan;//液晶屏的扫描模式（0-7）
	
	uint8_t light_mode;//光照模式，参数范围[0~5]
	int8_t saturation;//饱和度,参数范围[-4 ~ +4]   
	int8_t brightness;//光照度，参数范围[-4~+4]
	int8_t contrast;//对比度，参数范围[-4~+4]
	uint8_t effect;	//特殊效果，参数范围[0~6]:	


}OV7725_MODE_PARAM;


/* 寄存器宏定义 */
#define REG_GAIN      0x00
#define REG_BLUE      0x01
#define REG_RED       0x02
#define REG_GREEN     0x03
#define REG_BAVG      0x05
#define REG_GAVG      0x06
#define REG_RAVG      0x07
#define REG_AECH      0x08
#define REG_COM2      0x09
#define REG_PID       0x0A
#define REG_VER       0x0B
#define REG_COM3      0x0C
#define REG_COM4      0x0D
#define REG_COM5      0x0E
#define REG_COM6      0x0F
#define REG_AEC       0x10
#define REG_CLKRC     0x11
#define REG_COM7      0x12
#define REG_COM8      0x13
#define REG_COM9      0x14
#define REG_COM10     0x15
#define REG_REG16     0x16
#define REG_HSTART    0x17
#define REG_HSIZE     0x18
#define REG_VSTRT     0x19
#define REG_VSIZE     0x1A
#define REG_PSHFT     0x1B
#define REG_MIDH      0x1C
#define REG_MIDL      0x1D
#define REG_LAEC      0x1F
#define REG_COM11     0x20
#define REG_BDBase    0x22
#define REG_BDMStep   0x23
#define REG_AEW       0x24
#define REG_AEB       0x25
#define REG_VPT       0x26
#define REG_REG28     0x28
#define REG_HOutSize  0x29
#define REG_EXHCH     0x2A
#define REG_EXHCL     0x2B
#define REG_VOutSize  0x2C
#define REG_ADVFL     0x2D
#define REG_ADVFH     0x2E
#define REG_YAVE      0x2F
#define REG_LumHTh    0x30
#define REG_LumLTh    0x31
#define REG_HREF      0x32
#define REG_DM_LNL    0x33
#define REG_DM_LNH    0x34
#define REG_ADoff_B   0x35
#define REG_ADoff_R   0x36
#define REG_ADoff_Gb  0x37
#define REG_ADoff_Gr  0x38
#define REG_Off_B     0x39
#define REG_Off_R     0x3A
#define REG_Off_Gb    0x3B
#define REG_Off_Gr    0x3C
#define REG_COM12     0x3D
#define REG_COM13     0x3E
#define REG_COM14     0x3F
#define REG_COM16     0x41
#define REG_TGT_B     0x42
#define REG_TGT_R     0x43
#define REG_TGT_Gb    0x44
#define REG_TGT_Gr    0x45
#define REG_LC_CTR    0x46
#define REG_LC_XC     0x47
#define REG_LC_YC     0x48
#define REG_LC_COEF   0x49
#define REG_LC_RADI   0x4A
#define REG_LC_COEFB  0x4B 
#define REG_LC_COEFR  0x4C
#define REG_FixGain   0x4D
#define REG_AREF1     0x4F
#define REG_AREF6     0x54
#define REG_UFix      0x60
#define REG_VFix      0x61
#define REG_AWBb_blk  0x62
#define REG_AWB_Ctrl0 0x63
#define REG_DSP_Ctrl1 0x64
#define REG_DSP_Ctrl2 0x65
#define REG_DSP_Ctrl3 0x66
#define REG_DSP_Ctrl4 0x67
#define REG_AWB_bias  0x68
#define REG_AWBCtrl1  0x69
#define REG_AWBCtrl2  0x6A
#define REG_AWBCtrl3  0x6B
#define REG_AWBCtrl4  0x6C
#define REG_AWBCtrl5  0x6D
#define REG_AWBCtrl6  0x6E
#define REG_AWBCtrl7  0x6F
#define REG_AWBCtrl8  0x70
#define REG_AWBCtrl9  0x71
#define REG_AWBCtrl10 0x72
#define REG_AWBCtrl11 0x73
#define REG_AWBCtrl12 0x74
#define REG_AWBCtrl13 0x75
#define REG_AWBCtrl14 0x76
#define REG_AWBCtrl15 0x77
#define REG_AWBCtrl16 0x78
#define REG_AWBCtrl17 0x79
#define REG_AWBCtrl18 0x7A
#define REG_AWBCtrl19 0x7B
#define REG_AWBCtrl20 0x7C
#define REG_AWBCtrl21 0x7D 
#define REG_GAM1      0x7E
#define REG_GAM2      0x7F
#define REG_GAM3      0x80
#define REG_GAM4      0x81
#define REG_GAM5      0x82
#define REG_GAM6      0x83
#define REG_GAM7      0x84
#define REG_GAM8      0x85
#define REG_GAM9      0x86
#define REG_GAM10     0x87
#define REG_GAM11     0x88
#define REG_GAM12     0x89
#define REG_GAM13     0x8A
#define REG_GAM14     0x8B
#define REG_GAM15     0x8C
#define REG_SLOP      0x8D
#define REG_DNSTh     0x8E
#define REG_EDGE0     0x8F
#define REG_EDGE1     0x90
#define REG_DNSOff    0x91
#define REG_EDGE2     0x92
#define REG_EDGE3     0x93
#define REG_MTX1      0x94
#define REG_MTX2      0x95
#define REG_MTX3      0x96
#define REG_MTX4      0x97
#define REG_MTX5      0x98
#define REG_MTX6      0x99
#define REG_MTX_Ctrl  0x9A
#define REG_BRIGHT    0x9B
#define REG_CNST      0x9C
#define REG_UVADJ0    0x9E
#define REG_UVADJ1    0x9F
#define REG_SCAL0     0xA0
#define REG_SCAL1     0xA1
#define REG_SCAL2     0xA2
#define REG_SDE       0xA6
#define REG_USAT      0xA7
#define REG_VSAT      0xA8
#define REG_HUECOS    0xA9
#define REG_HUESIN    0xAA
#define REG_SIGN      0xAB
#define REG_DSPAuto   0xAC



/************************** OV7725 连接引脚定义********************************/
// FIFO 输出使能，即模块中的OE

#define      OV7725_OE_GPIO_PORT         GPIOA
#define      OV7725_OE_GPIO_PIN          GPIO_PIN_3

// FIFO 写复位
#define      OV7725_WRST_GPIO_PORT       GPIOC
#define      OV7725_WRST_GPIO_PIN        GPIO_PIN_4

// FIFO 读复位

#define      OV7725_RRST_GPIO_PORT       GPIOA
#define      OV7725_RRST_GPIO_PIN        GPIO_PIN_2

// FIFO 读时钟

#define      OV7725_RCLK_GPIO_PORT       GPIOC
#define      OV7725_RCLK_GPIO_PIN        GPIO_PIN_5

// FIFO 写使能
#define      OV7725_WE_GPIO_PORT         GPIOD
#define      OV7725_WE_GPIO_PIN          GPIO_PIN_3


// 8位数据口
#define      OV7725_DATA_GPIO_PORT       GPIOB
#define      OV7725_DATA_0_GPIO_PIN      GPIO_PIN_8
#define      OV7725_DATA_1_GPIO_PIN      GPIO_PIN_9
#define      OV7725_DATA_2_GPIO_PIN      GPIO_PIN_10
#define      OV7725_DATA_3_GPIO_PIN      GPIO_PIN_11
#define      OV7725_DATA_4_GPIO_PIN      GPIO_PIN_12
#define      OV7725_DATA_5_GPIO_PIN      GPIO_PIN_13
#define      OV7725_DATA_6_GPIO_PIN      GPIO_PIN_14
#define      OV7725_DATA_7_GPIO_PIN      GPIO_PIN_15

// OV7725场中断

#define      OV7725_VSYNC_GPIO_PORT      GPIOC
#define      OV7725_VSYNC_GPIO_PIN       GPIO_PIN_3

#define      OV7725_VSYNC_EXTI_IRQ            EXTI3_IRQn
#define      OV7725_VSYNC_EXTI_INT_FUNCTION   EXTI3_IRQHandler




#define FIFO_OE_H()     OV7725_OE_GPIO_PORT->BSRR =OV7725_OE_GPIO_PIN	  
#define FIFO_OE_L()     OV7725_OE_GPIO_PORT->BRR  =OV7725_OE_GPIO_PIN	  /*拉低使FIFO输出使能*/

#define FIFO_WRST_H()   OV7725_WRST_GPIO_PORT->BSRR =OV7725_WRST_GPIO_PIN	  /*拉高允许FIFO写(数据from摄像头)指针运动 */
#define FIFO_WRST_L()   OV7725_WRST_GPIO_PORT->BRR  =OV7725_WRST_GPIO_PIN	  /*拉低使FIFO写(数据from摄像头)指针复位*/

#define FIFO_RRST_H()   OV7725_RRST_GPIO_PORT->BSRR =OV7725_RRST_GPIO_PIN	  /*拉高允许FIFO读(数据从FIFO输出)指针运动 */
#define FIFO_RRST_L()   OV7725_RRST_GPIO_PORT->BRR  =OV7725_RRST_GPIO_PIN	  /*拉低使FIFO读(数据从FIFO输出)指针复位 */

#define FIFO_RCLK_H()   OV7725_RCLK_GPIO_PORT->BSRR =OV7725_RCLK_GPIO_PIN	  
#define FIFO_RCLK_L()   OV7725_RCLK_GPIO_PORT->BRR  =OV7725_RCLK_GPIO_PIN	  /*FIFO输出数据时钟*/

#define FIFO_WE_H()     OV7725_WE_GPIO_PORT->BSRR =OV7725_WE_GPIO_PIN	  /*拉高使FIFO写允许*/
#define FIFO_WE_L()     OV7725_WE_GPIO_PORT->BRR  =OV7725_WE_GPIO_PIN


#define READ_FIFO_PIXEL(RGB565)   	do{\
	                                  RGB565=0;\
	                                  FIFO_RCLK_L();\
	                                  RGB565 = (OV7725_DATA_GPIO_PORT->IDR) & 0xff00;\
	                                  FIFO_RCLK_H();\
                                    FIFO_RCLK_L();\
	                                  RGB565 |= (OV7725_DATA_GPIO_PORT->IDR >>8) & 0x00ff;\
	                                  FIFO_RCLK_H();\
                                    }while(0)

#define FIFO_PREPARE                do{\
	                                  FIFO_RRST_L();\
	                                  FIFO_RCLK_L();\
	                                  FIFO_RCLK_H();\
	                                  FIFO_RRST_H();\
	                                  FIFO_RCLK_L();\
	                                  FIFO_RCLK_H();\
                                    }while(0)

#define OV7725_ID       0x21
																		
																		
																		

/*信息输出*/
#define OV7725_DEBUG_ON         1

#define OV7725_INFO(fmt,arg...)           printf("<<-OV7725-INFO->> "fmt"\n",##arg)
#define OV7725_ERROR(fmt,arg...)          printf("<<-OV7725-ERROR->> "fmt"\n",##arg)
#define OV7725_DEBUG(fmt,arg...)          do{\
                                          if(OV7725_DEBUG_ON)\
                                          printf("<<-OV7725-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

																		

void OV7725_GPIO_Config(void);
ErrorStatus OV7725_Init(void);
void ImagDisp(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void OV7725_Light_Mode(uint8_t mode);
void OV7725_Color_Saturation(int8_t sat);
void OV7725_Brightness(int8_t bri);
void OV7725_Contrast(int8_t cnst);
void OV7725_Special_Effect(uint8_t eff);
void VSYNC_Init(void);				
void OV7725_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height,uint8_t QVGA_VGA);

#endif























