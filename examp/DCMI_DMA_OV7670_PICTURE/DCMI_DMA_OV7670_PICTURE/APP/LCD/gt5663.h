#ifndef _gt5663_H
#define _gt5663_H


#include "main.h"


//����ݴ��������ӵ�оƬ����(δ����IIC����) 
//IO��������	 	
#define     GT_RST_1  			HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_SET)//��λ����
#define     GT_RST_0  			HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_RESET)
#define     GT_INT_READ  		HAL_GPIO_ReadPin(T_PEN_GPIO_Port, T_PEN_Pin)//�ж�����	


//I2C��д����	
#define GT_CMD_WR 		0X28     	//д����0X28
#define GT_CMD_RD 		0X29		//������0X29
  
//GT5663 ���ּĴ������� 
#define GT_CTRL_REG 	0X8040   	//GT5663���ƼĴ���
#define GT_CFGS_REG 	0X8050   	//GT5663������ʼ��ַ�Ĵ���0X8050
#define GT_CHECK_REG 	0X8042   	//GT5663У��ͼĴ���0X8042
#define GT_PID_REG 		0X8140   	//GT5663��ƷID�Ĵ���

#define GT_GSTID_REG 	0X814E   	//GT5663��ǰ��⵽�Ĵ������
#define GT_TP1_REG 		0X8150  	//��һ�����������ݵ�ַ
#define GT_TP2_REG 		0X8158		//�ڶ������������ݵ�ַ
#define GT_TP3_REG 		0X8160		//���������������ݵ�ַ
#define GT_TP4_REG 		0X8168		//���ĸ����������ݵ�ַ
#define GT_TP5_REG 		0X8170		//��������������ݵ�ַ  
 
 
u8 GT5663_Send_Cfg(u8 mode);
u8 GT5663_WR_Reg(u16 reg,u8 *buf,u8 len);
void GT5663_RD_Reg(u16 reg,u8 *buf,u8 len); 
u8 GT5663_Init(void);
u8 GT5663_Scan(u8 mode);

#endif
