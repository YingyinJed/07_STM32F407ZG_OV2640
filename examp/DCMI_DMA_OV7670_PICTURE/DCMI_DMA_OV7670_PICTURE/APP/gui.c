#include "gui.h"
#include "LCD.h"
//����
//x,y:����
//FRONT_COLOR:�˵����ɫ
void gui_DrawPoint(u16 x,u16 y)
{
	LCD_Set_Window(x, y, x, y);  //���õ��λ��
	LCD_WriteData_Color(FRONT_COLOR);	
}
//���ٻ���
//x,y:����
//color:��ɫ
void gui_DrawFRONT_COLOR(u16 x,u16 y,u16 color)
{	   
    LCD_Set_Window(x, y, x, y);
	LCD_WriteData_Color(color);	
}
//����
//x1,y1:�������
//x2,y2:�յ�����  
void gui_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		gui_DrawPoint(uRow,uCol);//���� 
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
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		gui_DrawFRONT_COLOR(uRow,uCol,color);//���� 
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
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void gui_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	gui_DrawLine(x1,y1,x2,y1);
	gui_DrawLine(x1,y1,x1,y2);
	gui_DrawLine(x1,y2,x2,y2);
	gui_DrawLine(x2,y1,x2,y2);
}
//���һ������
//color:Ҫ���������ɫ
void gui_fill_Clear(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	uint16_t i, j ;

	LCD_Set_Window(x1, y1, x2+1, y2+1);	 //��������
  	for(i=x1; i<x2+1; i++)
	{
		for (j=y1; j<y2+1; j++)
		{
			LCD_WriteData_Color(color);
		}
	} 
}


//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void gui_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

