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
//-01                   ���г��������
//==============================================================

void startManage(void){//ֻ������ʱѭ��ִ�У��ڲ�������Ҫ���ϲ���
  SysTick_Init(168);
  TFTLCD_Init();
  KEY_Param_Init(&key); 
  USER_TIM7_Init(8399,99);  //��ʱ�����ڰ�������
  HAL_TIM_Base_Start_IT(&htim7);
  My_OV7670_Init();     //�ȴ�10�룬����ͼ����ʾ��LCD����
  My_DCMI_START();			//DCMI����,
}

void	board_quick(void){//run imidiately	
   TIM7_PROCESS();
   My_TAKE_A_PICTURE();
}
//==============================================================
//-02                   ����ִ�к���(����ֲ)
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
    if(_key->key_up ==KEY_JUST_PRESSED)//�����ϼ���ʼ���࣬LCDֹͣ���񣬲���LCD����ֹͣ��һ����Ƭͨ��uart1��������λ��
    {
      HAL_DCMI_Stop(&hdcmi);
      delay_ms(1000);
      rx_buffer[0]=0x01;               //��λ��Э�� 0X01 0XFE [����1][����2][...] 0XFE 0X01
      HAL_UART_Transmit(&huart1,rx_buffer,sizeof(rx_buffer),500);
      while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};
      rx_buffer[0]=0xFE;
      HAL_UART_Transmit(&huart1,rx_buffer,sizeof(rx_buffer),500);
      while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};
      flag_photo=1;

      
    }
    
    if(_key->key_dowm ==KEY_JUST_PRESSED)//�����¼����¿�ʼ����
    {
      My_DCMI_START();
      flag_photo=0;    
    }
   
}

//==============================================================
//-03                   ����ִ�к���(������ֲ)
//==============================================================

