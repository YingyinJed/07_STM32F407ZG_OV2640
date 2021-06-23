#ifndef _OV7670_H
#define _OV7670_H
#include "main.h"



#define OV7670_PWDN_0  	HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port,DCMI_PWDN_Pin,GPIO_PIN_RESET)			//POWER DOWN控制信号 
#define OV7670_PWDN_1  	HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port,DCMI_PWDN_Pin,GPIO_PIN_SET)
#define OV7670_RST_0  	HAL_GPIO_WritePin(DCMI_RESET_GPIO_Port,DCMI_RESET_Pin,GPIO_PIN_RESET)			//复位控制信号 
#define OV7670_RST_1  	HAL_GPIO_WritePin(DCMI_RESET_GPIO_Port,DCMI_RESET_Pin,GPIO_PIN_SET)	
////////////////////////////////////////////////////////////////////////////////// 
#define OV7670_MID				0X7FA2    
#define OV7670_PID				0X7673
													  					 
#define OV7670_DATA   GPIO_ReadInputData(GPIOC,0x00FF) 					//数据输入端口
//GPIOC->IDR&0x00FF 
/////////////////////////////////////////


void MCLK_ON(void);
void MCLK_OFF(void);
u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void set_cif(void);

#endif





















