#include "str.h"
#include "main.h"                         
#include "font.h"                        
#include "LCD.h"                         
#include  <string.h>

BUFFER_STRRING     str1;
//快速画点
//x,y:坐标
//color:颜色
void str_DrawFRONT_COLOR(u16 x,u16 y,u16 color)
{	   
    LCD_WR_REG(tftlcd_data.setxcmd);LCD_WR_DATA(x>>8);  
		LCD_WR_REG(tftlcd_data.setxcmd+1);LCD_WR_DATA(x&0XFF);	  
		LCD_WR_REG(tftlcd_data.setycmd);LCD_WR_DATA(y>>8);  
		LCD_WR_REG(tftlcd_data.setycmd+1);LCD_WR_DATA(y&0XFF); 
    TFTLCD->LCD_CMD=tftlcd_data.wramcmd; 
    TFTLCD->LCD_DATA=color; 
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void str_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=ascii_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=ascii_1608[num][t];	//调用1608字体
		else if(size==24)temp=ascii_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)str_DrawFRONT_COLOR(x,y,FRONT_COLOR);
			else if(mode==0)str_DrawFRONT_COLOR(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=tftlcd_data.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=tftlcd_data.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void str_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        str_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
