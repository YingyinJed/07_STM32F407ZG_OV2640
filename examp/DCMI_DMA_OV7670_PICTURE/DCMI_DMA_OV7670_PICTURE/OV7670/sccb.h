#ifndef __SCCB_H
#define __SCCB_H
#include "main.h"


//IO方向设置
#define SCCB_SDA_IN()  {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=0<<7*2;}	//PD7 输入
#define SCCB_SDA_OUT() {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=1<<7*2;} 	//PD7 输出


//IO操作函数	 
#define SCCB_SCL_0    		HAL_GPIO_WritePin(DCMI_SCL_GPIO_Port,DCMI_SCL_Pin,GPIO_PIN_RESET)	 	//SCL
#define SCCB_SCL_1    		HAL_GPIO_WritePin(DCMI_SCL_GPIO_Port,DCMI_SCL_Pin,GPIO_PIN_SET)
#define SCCB_SDA_0    		HAL_GPIO_WritePin(DCMI_SDA_GPIO_Port,DCMI_SDA_Pin,GPIO_PIN_RESET)	//SDA	 
#define SCCB_SDA_1    		HAL_GPIO_WritePin(DCMI_SDA_GPIO_Port,DCMI_SDA_Pin,GPIO_PIN_SET)

#define SCCB_READ_SDA     HAL_GPIO_ReadPin(DCMI_SDA_GPIO_Port,DCMI_SDA_Pin)  		//输入SDA    
#define SCCB_ID   			0X42  			//OV7670的ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif













