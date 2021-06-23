#ifndef _LCD_H
#define _LCD_H
#include "main.h"

//����LCD��������������  �ɸ����Լ����ϵĲ��������ͺ���ѡ�����������
//#define TFTLCD_HX8357D 

//#define TFTLCD_HX8352C

//#define TFTLCD_ILI9488

//#define TFTLCD_ST7793

//#define TFTLCD_ILI9327

#define TFTLCD_NT35510

//#define TFTLCD_ILI9486



#define TFTLCD_DIR	0	//0������  1������  Ĭ������

//TFTLCD��ַ�ṹ��
typedef struct
{
	u16 LCD_CMD;
	u16 LCD_DATA;
}TFTLCD_TypeDef;


//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E			    
#define TFTLCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)



//TFTLCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;            //LCD ����
  u16	wramcmd;		//��ʼдgramָ��
  u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ�� 
}_tftlcd_data;
//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//LCD����
extern _tftlcd_data tftlcd_data;	//����LCD��Ҫ����
extern u16  FRONT_COLOR;//ǰ����ɫ Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

void LCD_WriteCmd(u16 cmd);
void LCD_WriteData(u16 data);
void LCD_WriteCmdData(u16 cmd,u16 data);
void TFTLCD_Init(void);           //��ʼ��
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void LCD_Clear(u16 Color);	 												//����
void LCD_WriteData_Color(u16 color);

u16 LCD_ReadData(void);
void LCD_WriteRAM_Prepare(void);
void LCD_Scan_Dir(u8 dir);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_WR_REG(vu16 regval);
void LCD_WR_DATA(vu16 data);
u16 LCD_ReadPoint(u16 x,u16 y);
#endif
