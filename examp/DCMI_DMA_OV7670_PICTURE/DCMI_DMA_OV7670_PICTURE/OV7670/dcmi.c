#include "dcmi.h" 
#include "LCD.h" 
#include "ov7670.h" 
#include "LCD.h"
#include "SysTick.h"
#include "str.h"
#include "board.h"
#include "image.h"
#include "stdio.h"
#include "gui.h"
extern DCMI_HandleTypeDef hdcmi;
extern u8  rx_buffer[1];
extern UART_HandleTypeDef huart1;
u8 flag_photo=0;
//DCMI��ʼ��
void My_DCMI_START(void)
{
  
	LCD_SetCursor(0,0);  
	LCD_WriteRAM_Prepare();			//��ʼд��GRAM2
  HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(u32)&TFTLCD->LCD_DATA,1);

} 
void My_OV7670_Init(void)
{
  FRONT_COLOR=RED;//��������Ϊ��ɫ
	str_ShowString(30,50,200,16,16,(u8*)"Wait for a moment...");	
  OV7670_Init();
  str_ShowString(30,130,200,16,16,(u8*)"OV7670 OK"); 
	delay_ms(200);
  OV7670_Window_Set(12,176,240,320);//OV7670�����������
  LCD_Scan_Dir(U2D_L2R);		   //���ϵ���,������	
	LCD_Set_Window(0,0,320,240); //LCD������ʾ���ڣ�����ı��˷ֱ��ʣ�������Ҫ����
}

uint16_t LCD_RAM_FATA_BUFF;
void My_TAKE_A_PICTURE(void)
{
  
  if(flag_photo==1)
  {
    int x,y=0;
    for(x=0;x<320;x++)
    {
      for(y=0;y<240;y++)
      {
        LCD_RAM_FATA_BUFF=LCD_ReadPoint(x,y);
        HAL_UART_Transmit(&huart1,(u8 *)&LCD_RAM_FATA_BUFF,sizeof(LCD_RAM_FATA_BUFF),500);
        while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};//�ȴ�һ�����ص㴫�����
        //һ��ͼƬ�ܹ�320*240*2���ֽ�
        //����λ������������256000bits������һ��ͼƬ������Ҫ����8�룬�����ĵȴ���
      }
    }
      rx_buffer[0]=0xFE;//��λ��Э��--ĩβ
      HAL_UART_Transmit(&huart1,rx_buffer,sizeof(rx_buffer),500);
      while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};
      rx_buffer[0]=0x01;
      HAL_UART_Transmit(&huart1,rx_buffer,sizeof(rx_buffer),500);
      while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};
      flag_photo=0;


  }


}
