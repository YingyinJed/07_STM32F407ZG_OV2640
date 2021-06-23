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
//DCMI初始化
void My_DCMI_START(void)
{
  
	LCD_SetCursor(0,0);  
	LCD_WriteRAM_Prepare();			//开始写入GRAM2
  HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(u32)&TFTLCD->LCD_DATA,1);

} 
void My_OV7670_Init(void)
{
  FRONT_COLOR=RED;//设置字体为红色
	str_ShowString(30,50,200,16,16,(u8*)"Wait for a moment...");	
  OV7670_Init();
  str_ShowString(30,130,200,16,16,(u8*)"OV7670 OK"); 
	delay_ms(200);
  OV7670_Window_Set(12,176,240,320);//OV7670设置输出窗口
  LCD_Scan_Dir(U2D_L2R);		   //从上到下,从左到右	
	LCD_Set_Window(0,0,320,240); //LCD设置显示窗口，如果改变了分辨率，这里需要更改
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
        while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};//等待一个像素点传输完毕
        //一张图片总共320*240*2个字节
        //而上位机波特率上限256000bits，所以一张图片传输需要将近8秒，请耐心等待。
      }
    }
      rx_buffer[0]=0xFE;//上位机协议--末尾
      HAL_UART_Transmit(&huart1,rx_buffer,sizeof(rx_buffer),500);
      while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};
      rx_buffer[0]=0x01;
      HAL_UART_Transmit(&huart1,rx_buffer,sizeof(rx_buffer),500);
      while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET){};
      flag_photo=0;


  }


}
