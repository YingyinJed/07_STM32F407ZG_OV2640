#ifndef _touch_H
#define _touch_H

#include "main.h"
#include "LCD.h"

#define CT_MAX_TOUCH 10	//�����������10������

/* ������������ */
typedef struct 
{
	uint16_t x;
	uint16_t y;
  uint16_t lcdx[CT_MAX_TOUCH];
  uint16_t lcdy[CT_MAX_TOUCH];
	uint8_t tpsta;
} TouchTypeDef;

extern TouchTypeDef TouchData;

typedef struct{
    uint8_t posState;   
    int16_t xOffset;
    int16_t yOffset; 
    float xFactor;
    float yFactor;
} PosTypeDef;
///////////////////////////////
typedef  enum{
    NO_TOUCH   = 0,    // ������û�а���
    JUST_TOUCH    ,    // �������հ���
    TOUCHED       ,    // �������Ѿ���������
    JUST_POP      ,    // �������յ���
}TouchMainStatus;

typedef  enum{  
    Y_NO_MOVING=0 ,
    UP_MOVING     ,   // �������Ѿ����£����������ƶ�
    DOWN_MOVING   ,   // �������Ѿ����£����������ƶ�
}YSubStatus;

typedef  enum{ 
    X_NO_MOVING=0 ,
    LEFT_MOVING   ,   // �������Ѿ����£����������ƶ�
    RIGHT_MOVING  ,   // �������Ѿ����£����������ƶ�
}XSubStatus;

typedef struct 
{
    uint16_t ctp_x;                  // ��������X����ֵ
    uint16_t ctp_y;                  // ��������Y����ֵ
}struCTPxy;

typedef  struct
{
    uint8_t         touch_flag;
    struCTPxy       ctpxy;           // ������������XY
    TouchMainStatus ctpmainstatus;   // ����������Ҫ״̬
    XSubStatus      xmove;           // X������ƶ�״̬
    YSubStatus      ymove;           // Y������ƶ�״̬
    int16_t         dx;              // X�����ƶ������� +ֵ:��ʾ�����ƶ�; -ֵ:��ʾ�����ƶ�
    int16_t         dy;              // Y�����ƶ������� +ֵ:��ʾ�����ƶ�; -ֵ:��ʾ�����ƶ�
}struTouch;

#define TOUCH_ADJ_OK          'Y'              //��ʾ����У������׼����
#define TOUCH_ADJ_ADDR        200          //У��������24C02�е��׵�ַ200
/* ����У���������� */
#define LCD_ADJX_MIN (10)                      //��ȡ�ĸ������СXֵ
#define LCD_ADJX_MAX (tftlcd_data.width - LCD_ADJX_MIN) //��ȡ�ĸ�������Xֵ
#define LCD_ADJY_MIN (10)                      //��ȡ�ĸ������СYֵ
#define LCD_ADJY_MAX (tftlcd_data.height - LCD_ADJY_MIN) //��ȡ�ĸ�������Yֵ

#define LCD_ADJ_X (LCD_ADJX_MAX - LCD_ADJY_MIN)//��ȡ����Ŀ��
#define LCD_ADJ_Y (LCD_ADJY_MAX - LCD_ADJY_MIN)//��ȡ����ĸ߶�

#define TOUCH_READ_TIMES 40     //һ�ζ�ȡ����ֵ�Ĵ���

#define TOUCH_X_CMD      0xD0  //��ȡX������
#define TOUCH_Y_CMD      0x90  //��ȡY������
#define TOUCH_MAX        20    //Ԥ�ڲ�ֵ
#define TOUCH_X_MAX      4000  //X�����ֵ
#define TOUCH_X_MIN      100   //X����Сֵ
#define TOUCH_Y_MAX      4000  //Y�����ֵ
#define TOUCH_Y_MIN      100   //Y����Сֵ


//������оƬ��������	   
#define     T_CLK_0  			HAL_GPIO_WritePin(T_SCK_GPIO_Port, T_SCK_Pin, GPIO_PIN_RESET)
#define     T_CLK_1  			HAL_GPIO_WritePin(T_SCK_GPIO_Port, T_SCK_Pin, GPIO_PIN_SET)
#define     T_DIN_0  			HAL_GPIO_WritePin(T_MOSI_GPIO_Port, T_MOSI_Pin, GPIO_PIN_RESET)
#define     T_DIN_1  			HAL_GPIO_WritePin(T_MOSI_GPIO_Port, T_MOSI_Pin, GPIO_PIN_SET)
#define     T_TCS_0  			HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_RESET)
#define     T_TCS_1  			HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_SET)
#define     T_PEN_READ  	HAL_GPIO_ReadPin(T_PEN_GPIO_Port, T_PEN_Pin)
#define     T_DOUT_READ  	HAL_GPIO_ReadPin(T_MISO_GPIO_Port, T_MISO_Pin)
void TOUCH_Init(void);

void  ReadCTP(struTouch *nowctp);
void  Get_Test_Val(struTouch  nowctp, uint16_t *val_x, uint16_t *val_y);
uint8_t TOUCH_Scan(void);

#endif
