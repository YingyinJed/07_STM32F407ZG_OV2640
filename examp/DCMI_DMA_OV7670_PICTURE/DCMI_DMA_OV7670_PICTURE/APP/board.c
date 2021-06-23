#include "board.h"
#include "uart.h"
#include "main.h"
#include "LCD.h"
#include "SysTick.h"
#include "str.h"
#include "dcmi.h"
#include "tim.h"
#include "stdio.h"
#include "gui.h"
extern struKEY  key;
extern FunctionalState  TIM7_EN;
extern TIM_HandleTypeDef htim7;
extern DCMI_HandleTypeDef hdcmi;
extern UART_HandleTypeDef huart1;
extern u8 flag_photo;
u8  rx_buffer[1];
//==============================================================
//-01                   运行程序管理函数
//==============================================================

void startManage(void){//只在启动时循环执行，内部功能需要不断补充
  SysTick_Init(168);
  TFTLCD_Init();
  KEY_Param_Init(&key); 
  USER_TIM7_Init(8399,99);  //定时器用于按键消抖
  HAL_TIM_Base_Start_IT(&htim7);
  My_OV7670_Init();     //等待10秒，连续图像显示在LCD屏上
  My_DCMI_START();			//DCMI配置,
}

void	board_quick(void){//run imidiately	
   TIM7_PROCESS();
   My_TAKE_A_PICTURE();
}
//==============================================================
//-02                   具体执行函数(可移植)
//==============================================================

void TIM7_PROCESS(void)
{
  if(TIM7_EN == ENABLE)
  {  
      Key_Read(&key);
      Key_Process(&key);
      TIM7_EN = DISABLE;
  }
}

void  Key_Process(struKEY *_key)
{
    if(_key->key_up ==KEY_JUST_PRESSED)//按下上键开始照相，LCD停止摄像，并将LCD屏上停止的一张照片通过uart1传输至上位机
    {
      HAL_DCMI_Stop(&hdcmi);
      delay_ms(1000);
      rx_buffer[0]=0x01;               //上位机协议 0X01 0XFE [数据1][数据2][...] 0XFE 0X01
      HAL_UART_Transmit(&huart1,rx_buffer,sizeof(rx_buffer),500);
      while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};
      rx_buffer[0]=0xFE;
      HAL_UART_Transmit(&huart1,rx_buffer,sizeof(rx_buffer),500);
      while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};
      flag_photo=1;

      
    }
    
    if(_key->key_dowm ==KEY_JUST_PRESSED)//按下下键重新开始摄像
    {
      My_DCMI_START();
      flag_photo=0;    
    }
   
}

//==============================================================
//-03                   具体执行函数(不可移植)
//==============================================================

