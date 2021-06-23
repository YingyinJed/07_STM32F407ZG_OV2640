#ifndef _touch_H
#define _touch_H

#include "main.h"
#include "LCD.h"

#define CT_MAX_TOUCH 10	//电容屏有最多10组坐标

/* 定义数据类型 */
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
    NO_TOUCH   = 0,    // 触摸屏没有按下
    JUST_TOUCH    ,    // 触摸屏刚按下
    TOUCHED       ,    // 触摸屏已经触摸到了
    JUST_POP      ,    // 触摸屏刚弹出
}TouchMainStatus;

typedef  enum{  
    Y_NO_MOVING=0 ,
    UP_MOVING     ,   // 触摸屏已经按下，并且向上移动
    DOWN_MOVING   ,   // 触摸屏已经按下，并且向下移动
}YSubStatus;

typedef  enum{ 
    X_NO_MOVING=0 ,
    LEFT_MOVING   ,   // 触摸屏已经按下，并且向左移动
    RIGHT_MOVING  ,   // 触摸屏已经按下，并且向右移动
}XSubStatus;

typedef struct 
{
    uint16_t ctp_x;                  // 触摸屏的X坐标值
    uint16_t ctp_y;                  // 触摸屏的Y坐标值
}struCTPxy;

typedef  struct
{
    uint8_t         touch_flag;
    struCTPxy       ctpxy;           // 触摸屏的坐标XY
    TouchMainStatus ctpmainstatus;   // 触摸屏的主要状态
    XSubStatus      xmove;           // X方向的移动状态
    YSubStatus      ymove;           // Y方向的移动状态
    int16_t         dx;              // X方向移动的像素 +值:表示向右移动; -值:表示向左移动
    int16_t         dy;              // Y方向移动的像素 +值:表示向下移动; -值:表示向上移动
}struTouch;

#define TOUCH_ADJ_OK          'Y'              //表示触摸校正参数准备好
#define TOUCH_ADJ_ADDR        200          //校正参数在24C02中的首地址200
/* 触摸校正因数设置 */
#define LCD_ADJX_MIN (10)                      //读取四个点的最小X值
#define LCD_ADJX_MAX (tftlcd_data.width - LCD_ADJX_MIN) //读取四个点的最大X值
#define LCD_ADJY_MIN (10)                      //读取四个点的最小Y值
#define LCD_ADJY_MAX (tftlcd_data.height - LCD_ADJY_MIN) //读取四个点的最大Y值

#define LCD_ADJ_X (LCD_ADJX_MAX - LCD_ADJY_MIN)//读取方框的宽度
#define LCD_ADJ_Y (LCD_ADJY_MAX - LCD_ADJY_MIN)//读取方框的高度

#define TOUCH_READ_TIMES 40     //一次读取触摸值的次数

#define TOUCH_X_CMD      0xD0  //读取X轴命令
#define TOUCH_Y_CMD      0x90  //读取Y轴命令
#define TOUCH_MAX        20    //预期差值
#define TOUCH_X_MAX      4000  //X轴最大值
#define TOUCH_X_MIN      100   //X轴最小值
#define TOUCH_Y_MAX      4000  //Y轴最大值
#define TOUCH_Y_MIN      100   //Y轴最小值


//电阻屏芯片连接引脚	   
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
