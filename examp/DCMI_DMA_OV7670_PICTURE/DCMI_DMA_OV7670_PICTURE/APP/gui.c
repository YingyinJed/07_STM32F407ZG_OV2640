#include "gui.h"
#include "LCD.h"
//画点
//x,y:坐标
//FRONT_COLOR:此点的颜色
void gui_DrawPoint(u16 x,u16 y)
{
	LCD_Set_Window(x, y, x, y);  //设置点的位置
	LCD_WriteData_Color(FRONT_COLOR);	
}
//快速画点
//x,y:坐标
//color:颜色
void gui_DrawFRONT_COLOR(u16 x,u16 y,u16 color)
{	   
    LCD_Set_Window(x, y, x, y);
	LCD_WriteData_Color(color);	
}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void gui_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  

void gui_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_DrawFRONT_COLOR(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void gui_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	gui_DrawLine(x1,y1,x2,y1);
	gui_DrawLine(x1,y1,x1,y2);
	gui_DrawLine(x1,y2,x2,y2);
	gui_DrawLine(x2,y1,x2,y2);
}
//填充一个矩形
//color:要清屏的填充色
void gui_fill_Clear(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	uint16_t i, j ;

	LCD_Set_Window(x1, y1, x2+1, y2+1);	 //作用区域
  	for(i=x1; i<x2+1; i++)
	{
		for (j=y1; j<y2+1; j++)
		{
			LCD_WriteData_Color(color);
		}
	} 
}


//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void gui_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		gui_DrawPoint(x0+a,y0-b);             //5
 		gui_DrawPoint(x0+b,y0-a);             //0           
		gui_DrawPoint(x0+b,y0+a);             //4               
		gui_DrawPoint(x0+a,y0+b);             //6 
		gui_DrawPoint(x0-a,y0+b);             //1       
 		gui_DrawPoint(x0-b,y0+a);             
		gui_DrawPoint(x0-a,y0-b);             //2             
  	gui_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

