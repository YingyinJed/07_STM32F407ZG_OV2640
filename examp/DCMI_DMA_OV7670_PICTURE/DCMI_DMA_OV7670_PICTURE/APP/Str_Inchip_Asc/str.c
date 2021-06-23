#include "str.h"
#include "main.h"                         
#include "font.h"                        
#include "LCD.h"                         
#include  <string.h>

BUFFER_STRRING     str1;
//���ٻ���
//x,y:����
//color:��ɫ
void str_DrawFRONT_COLOR(u16 x,u16 y,u16 color)
{	   
    LCD_WR_REG(tftlcd_data.setxcmd);LCD_WR_DATA(x>>8);  
		LCD_WR_REG(tftlcd_data.setxcmd+1);LCD_WR_DATA(x&0XFF);	  
		LCD_WR_REG(tftlcd_data.setycmd);LCD_WR_DATA(y>>8);  
		LCD_WR_REG(tftlcd_data.setycmd+1);LCD_WR_DATA(y&0XFF); 
    TFTLCD->LCD_CMD=tftlcd_data.wramcmd; 
    TFTLCD->LCD_DATA=color; 
}

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void str_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=ascii_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=ascii_1608[num][t];	//����1608����
		else if(size==24)temp=ascii_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)str_DrawFRONT_COLOR(x,y,FRONT_COLOR);
			else if(mode==0)str_DrawFRONT_COLOR(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=tftlcd_data.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=tftlcd_data.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void str_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        str_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
