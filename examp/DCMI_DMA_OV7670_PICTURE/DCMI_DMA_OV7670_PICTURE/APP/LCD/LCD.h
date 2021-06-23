#ifndef _LCD_H
#define _LCD_H
#include "main.h"

//定义LCD彩屏的驱动类型  可根据自己手上的彩屏背面型号来选择打开哪种驱动
//#define TFTLCD_HX8357D 

//#define TFTLCD_HX8352C

//#define TFTLCD_ILI9488

//#define TFTLCD_ST7793

//#define TFTLCD_ILI9327

#define TFTLCD_NT35510

//#define TFTLCD_ILI9486



#define TFTLCD_DIR	0	//0：竖屏  1：横屏  默认竖屏

//TFTLCD地址结构体
typedef struct
{
	u16 LCD_CMD;
	u16 LCD_DATA;
}TFTLCD_TypeDef;


//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A6作为数据命令区分线 
//注意设置时STM32内部会右移一位对齐! 111 1110=0X7E			    
#define TFTLCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)



//TFTLCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;            //LCD 方向
  u16	wramcmd;		//开始写gram指令
  u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_tftlcd_data;
//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//LCD参数
extern _tftlcd_data tftlcd_data;	//管理LCD重要参数
extern u16  FRONT_COLOR;//前端颜色 默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

void LCD_WriteCmd(u16 cmd);
void LCD_WriteData(u16 data);
void LCD_WriteCmdData(u16 cmd,u16 data);
void TFTLCD_Init(void);           //初始化
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void LCD_Clear(u16 Color);	 												//清屏
void LCD_WriteData_Color(u16 color);

u16 LCD_ReadData(void);
void LCD_WriteRAM_Prepare(void);
void LCD_Scan_Dir(u8 dir);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_WR_REG(vu16 regval);
void LCD_WR_DATA(vu16 data);
u16 LCD_ReadPoint(u16 x,u16 y);
#endif
