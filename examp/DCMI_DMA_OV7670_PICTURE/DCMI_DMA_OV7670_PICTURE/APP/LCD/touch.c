#include "touch.h"
#include "SysTick.h"
#include "LCD.h"
#include "gt5663.h"


#define TOUCH_AdjDelay500ms() delay_ms(500)

TouchTypeDef TouchData;         //���������洢��ȡ��������
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
		T_CLK_1;		//��������Ч	        
	}		 			    
}
 
u16 TOUCH_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	T_CLK_0;		//������ʱ�� 	 
	T_DIN_0; 	//����������
	T_TCS_0; 		//ѡ�д�����IC
	TOUCH_Write_Byte(CMD);//����������
	delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
	T_CLK_0; 	     	    
	delay_us(1);    	   
	T_CLK_1;		//��1��ʱ�ӣ����BUSY
	delay_us(1);    
	T_CLK_0; 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Num<<=1; 	 
		T_CLK_0;	//�½�����Ч  	    	   
		delay_us(1);    
 		T_CLK_1;
 		if(T_DOUT_READ)Num++; 		 
	}  	
	Num>>=4;   	//ֻ�и�12λ��Ч.
	T_TCS_1;		//�ͷ�Ƭѡ	 
	return(Num);   
}

uint16_t TOUCH_ReadData(uint8_t cmd)
{
    uint8_t i, j;
    uint16_t readValue[TOUCH_READ_TIMES], value;
    uint32_t totalValue;

    /* ��ȡTOUCH_READ_TIMES�δ���ֵ */
    for(i=0; i<TOUCH_READ_TIMES; i++)
    {   /* ��Ƭѡ */
        T_TCS_0;
        /* �ڲ��ģʽ�£�XPT2046ת����Ҫ24��ʱ�ӣ�8��ʱ���������֮��1��ʱ��ȥ�� */
        /* æ�źţ��������12λת�������ʣ��3��ʱ���Ǻ���λ */    
        readValue[i]=TOUCH_Read_AD(cmd); // �������ѡ��X�����Y�� 
        
        T_TCS_1;
    }

    /* �˲����� */
    /* ���ȴӴ�С���� */
    for(i=0; i<(TOUCH_READ_TIMES - 1); i++)
    {
        for(j=i+1; j<TOUCH_READ_TIMES; j++)
        {
            /* ����ֵ�Ӵ�С�������� */
            if(readValue[i] < readValue[j])
            {
                value = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = value;
            }   
        }       
    }
   
    /* ȥ�����ֵ��ȥ����Сֵ����ƽ��ֵ */
    j = TOUCH_READ_TIMES - 1;
    totalValue = 0;
    for(i=1; i<j; i++)     //��y��ȫ��ֵ
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
    
    /* �鿴������֮���ֻ����ֵ��� */
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

    /* �жϲ�����ֵ�Ƿ��ڿɿط�Χ�� */
	if((*xValue > TOUCH_MAX+0) || (*yValue > TOUCH_MAX+0))  
	{
		return 0xFF;
	}

    /* ��ƽ��ֵ */
    *xValue = (xValue1 + xValue2) / 2;
    *yValue = (yValue1 + yValue2) / 2;

    /* �жϵõ���ֵ���Ƿ���ȡֵ��Χ֮�� */
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

    // ��ȡ���ڵ�����ֵ
    nowctp->touch_flag = TOUCH_Scan() ;
    if(nowctp->touch_flag == 0)
    {
      nowctp->ctpxy.ctp_x=TouchData.lcdx[0];
      nowctp->ctpxy.ctp_y=TouchData.lcdy[0];
    }
    
    // ������һ�ε�״̬�����ڵ�����ֵ�ж����ڵ���Ҫ״̬
    if(ctplast.ctpmainstatus == NO_TOUCH)        // ��һ��û�д���
    {
        if( nowctp->touch_flag == 1) { nowctp->ctpmainstatus = NO_TOUCH;  }// ����Ҳû�д��� 
        else                         { nowctp->ctpmainstatus = JUST_TOUCH;}// ���ڸմ���  
    }
    else if((ctplast.ctpmainstatus == JUST_TOUCH) || (ctplast.ctpmainstatus == TOUCHED))   // ��һ�θմ��� �� �Ѿ���������
    {
        if( nowctp->touch_flag == 1) { nowctp->ctpmainstatus = JUST_POP;}  // �յ��� 
        else                         { nowctp->ctpmainstatus = TOUCHED; }  // �Ѿ����� 
    }
    else if(ctplast.ctpmainstatus == JUST_POP)
    {
        if( nowctp->touch_flag == 1) { nowctp->ctpmainstatus = NO_TOUCH;  }// û�а���  
        else                         { nowctp->ctpmainstatus = JUST_TOUCH;}// 
    }
    
    // �����ڵ���Ҫ״̬���ڰ���ʱ�����ж��ƶ����
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
        tmp += nowctp.dx;          // �����ƶ�����/�ƶ���ֵ/ ; �����ƶ�,��/�ƶ���ֵ/
        if(tmp < 0) tmp = 65535;
        if(tmp > 65535) tmp = 0;
        *val_x = (uint16_t)tmp;
        
        tmp  = (int)(*val_y);
        tmp -= nowctp.dy;          // �����ƶ�����/�ƶ���ֵ/ ; �����ƶ�,��/�ƶ���ֵ/
        if(tmp < 0) tmp = 65535;
        if(tmp > 65535) tmp = 0;
        *val_y = (uint16_t)tmp;
    }
}

