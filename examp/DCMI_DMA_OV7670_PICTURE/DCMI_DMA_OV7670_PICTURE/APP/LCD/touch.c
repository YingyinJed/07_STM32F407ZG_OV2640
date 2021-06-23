#include "touch.h"
#include "SysTick.h"
#include "LCD.h"
#include "gt5663.h"


#define TOUCH_AdjDelay500ms() delay_ms(500)

TouchTypeDef TouchData;         //定义用来存储读取到的数据
struTouch   touch;

void TOUCH_Init(void)
{
#ifdef TFTLCD_NT35510
	GT5663_Init();
#endif
	
}
void TOUCH_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)T_DIN_1;  
		else T_DIN_0;   
		num<<=1;    
		T_CLK_0; 
		delay_us(1);
		T_CLK_1;		//上升沿有效	        
	}		 			    
}
 
u16 TOUCH_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	T_CLK_0;		//先拉低时钟 	 
	T_DIN_0; 	//拉低数据线
	T_TCS_0; 		//选中触摸屏IC
	TOUCH_Write_Byte(CMD);//发送命令字
	delay_us(6);//ADS7846的转换时间最长为6us
	T_CLK_0; 	     	    
	delay_us(1);    	   
	T_CLK_1;		//给1个时钟，清除BUSY
	delay_us(1);    
	T_CLK_0; 	     	    
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效 
	{ 				  
		Num<<=1; 	 
		T_CLK_0;	//下降沿有效  	    	   
		delay_us(1);    
 		T_CLK_1;
 		if(T_DOUT_READ)Num++; 		 
	}  	
	Num>>=4;   	//只有高12位有效.
	T_TCS_1;		//释放片选	 
	return(Num);   
}

uint16_t TOUCH_ReadData(uint8_t cmd)
{
    uint8_t i, j;
    uint16_t readValue[TOUCH_READ_TIMES], value;
    uint32_t totalValue;

    /* 读取TOUCH_READ_TIMES次触摸值 */
    for(i=0; i<TOUCH_READ_TIMES; i++)
    {   /* 打开片选 */
        T_TCS_0;
        /* 在差分模式下，XPT2046转换需要24个时钟，8个时钟输入命令，之后1个时钟去除 */
        /* 忙信号，接着输出12位转换结果，剩下3个时钟是忽略位 */    
        readValue[i]=TOUCH_Read_AD(cmd); // 发送命令，选择X轴或者Y轴 
        
        T_TCS_1;
    }

    /* 滤波处理 */
    /* 首先从大到小排序 */
    for(i=0; i<(TOUCH_READ_TIMES - 1); i++)
    {
        for(j=i+1; j<TOUCH_READ_TIMES; j++)
        {
            /* 采样值从大到小排序排序 */
            if(readValue[i] < readValue[j])
            {
                value = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = value;
            }   
        }       
    }
   
    /* 去掉最大值，去掉最小值，求平均值 */
    j = TOUCH_READ_TIMES - 1;
    totalValue = 0;
    for(i=1; i<j; i++)     //求y的全部值
    {
        totalValue += readValue[i];
    }
    value = totalValue / (TOUCH_READ_TIMES - 2);
      
    return value;
}

uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{   
    uint16_t xValue1, yValue1, xValue2, yValue2;

    xValue1 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue1 = TOUCH_ReadData(TOUCH_Y_CMD);
    xValue2 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue2 = TOUCH_ReadData(TOUCH_Y_CMD);
    
    /* 查看两个点之间的只采样值差距 */
    if(xValue1 > xValue2)
    {
        *xValue = xValue1 - xValue2;
    }
    else
    {
        *xValue = xValue2 - xValue1;
    }

    if(yValue1 > yValue2)
    {
        *yValue = yValue1 - yValue2;
    }
    else
    {
        *yValue = yValue2 - yValue1;
    }

    /* 判断采样差值是否在可控范围内 */
	if((*xValue > TOUCH_MAX+0) || (*yValue > TOUCH_MAX+0))  
	{
		return 0xFF;
	}

    /* 求平均值 */
    *xValue = (xValue1 + xValue2) / 2;
    *yValue = (yValue1 + yValue2) / 2;

    /* 判断得到的值，是否在取值范围之内 */
    if((*xValue > TOUCH_X_MAX+0) || (*xValue < TOUCH_X_MIN) 
       || (*yValue > TOUCH_Y_MAX+0) || (*yValue < TOUCH_Y_MIN))
    {                   
        return 0xFF;
    }
 
    return 0; 
}
uint8_t TOUCH_Scan(void)
{

#ifdef TFTLCD_NT35510
	return GT5663_Scan(0);
#endif
	
}
static  struTouch  ctplast={0,0,0,NO_TOUCH,X_NO_MOVING,Y_NO_MOVING,};
void  ReadCTP(struTouch *nowctp)
{
    ctplast.ctpxy.ctp_x   = nowctp->ctpxy.ctp_x;
    ctplast.ctpxy.ctp_y   = nowctp->ctpxy.ctp_y;
    ctplast.ctpmainstatus = nowctp->ctpmainstatus;
    ctplast.dx            = nowctp->dx;
    ctplast.dy            = nowctp->dy;

    // 读取现在的坐标值
    nowctp->touch_flag = TOUCH_Scan() ;
    if(nowctp->touch_flag == 0)
    {
      nowctp->ctpxy.ctp_x=TouchData.lcdx[0];
      nowctp->ctpxy.ctp_y=TouchData.lcdy[0];
    }
    
    // 根据上一次的状态和现在的坐标值判断现在的主要状态
    if(ctplast.ctpmainstatus == NO_TOUCH)        // 上一次没有触摸
    {
        if( nowctp->touch_flag == 1) { nowctp->ctpmainstatus = NO_TOUCH;  }// 现在也没有触摸 
        else                         { nowctp->ctpmainstatus = JUST_TOUCH;}// 现在刚触摸  
    }
    else if((ctplast.ctpmainstatus == JUST_TOUCH) || (ctplast.ctpmainstatus == TOUCHED))   // 上一次刚触摸 或 已经触摸到了
    {
        if( nowctp->touch_flag == 1) { nowctp->ctpmainstatus = JUST_POP;}  // 刚弹出 
        else                         { nowctp->ctpmainstatus = TOUCHED; }  // 已经按下 
    }
    else if(ctplast.ctpmainstatus == JUST_POP)
    {
        if( nowctp->touch_flag == 1) { nowctp->ctpmainstatus = NO_TOUCH;  }// 没有按下  
        else                         { nowctp->ctpmainstatus = JUST_TOUCH;}// 
    }
    
    // 当现在的主要状态处于按下时，则判断移动情况
    if(nowctp->ctpmainstatus == TOUCHED)
    {
        nowctp->dx = (int16_t)nowctp->ctpxy.ctp_x - (int16_t)ctplast.ctpxy.ctp_x;
        if(nowctp->dx == 0)     { nowctp->xmove = X_NO_MOVING;  }
        else if(nowctp->dx > 0) { nowctp->xmove = RIGHT_MOVING; }
        else if(nowctp->dx < 0) { nowctp->xmove = LEFT_MOVING;  }
        
        nowctp->dy = (int16_t)nowctp->ctpxy.ctp_y - (int16_t)ctplast.ctpxy.ctp_y;
        if(nowctp->dy == 0)     { nowctp->ymove = Y_NO_MOVING;  }
        else if(nowctp->dy > 0) { nowctp->ymove = DOWN_MOVING;  }
        else if(nowctp->dy < 0) { nowctp->ymove = UP_MOVING;    }
    }
    else
    {
        nowctp->xmove = X_NO_MOVING;
        nowctp->ymove = Y_NO_MOVING;
        nowctp->dx = 0;
        nowctp->dy = 0;
    }
	 
}

void  Get_Test_Val(struTouch  nowctp, uint16_t *val_x, uint16_t *val_y)
{
    int    tmp;

    if(nowctp.ctpmainstatus == TOUCHED)
    {    
        tmp  = (int)(*val_x);
        tmp += nowctp.dx;          // 向右移动，加/移动差值/ ; 向左移动,减/移动差值/
        if(tmp < 0) tmp = 65535;
        if(tmp > 65535) tmp = 0;
        *val_x = (uint16_t)tmp;
        
        tmp  = (int)(*val_y);
        tmp -= nowctp.dy;          // 向上移动，加/移动差值/ ; 向下移动,减/移动差值/
        if(tmp < 0) tmp = 65535;
        if(tmp > 65535) tmp = 0;
        *val_y = (uint16_t)tmp;
    }
}

