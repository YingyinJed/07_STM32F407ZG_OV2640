#include "sccb.h"
#include "SysTick.h"
//初始化SCCB接口 
void SCCB_Init(void)
{				
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitStruct.Pin = DCMI_SCL_Pin|DCMI_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_WritePin(DCMI_SCL_GPIO_Port,DCMI_SCL_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(DCMI_SDA_GPIO_Port,DCMI_SDA_Pin,GPIO_PIN_SET);
	SCCB_SDA_OUT();	   
}			 

//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
    SCCB_SDA_1;     //数据线高电平	 
    delay_us(500); 
    SCCB_SCL_1;	    //在时钟线高的时候数据线由高至低
    delay_us(500);  
    SCCB_SDA_0;
    delay_us(500);	 
    SCCB_SCL_0;	    //数据线恢复低电平，单操作函数必要	
    delay_us(500);   
}

//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
    SCCB_SDA_0;
    delay_us(500);	 
    SCCB_SCL_1;	
    delay_us(500); 
    SCCB_SDA_1;	
    delay_us(500);
}  
//产生NA信号
void SCCB_No_Ack(void)
{
	delay_us(500);
	SCCB_SDA_1;	
  delay_us(500); 
	SCCB_SCL_1;	
	delay_us(500);
	SCCB_SCL_0;	
	delay_us(500);
	SCCB_SDA_0;	
	delay_us(500);
}
//SCCB,写入一个字节
//返回值:0,成功;1,失败. 
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)SCCB_SDA_1;	
		else SCCB_SDA_0;
		dat<<=1;
		delay_us(500);
		SCCB_SCL_1;	
		delay_us(500);
		SCCB_SCL_0;		   
	}			 
	SCCB_SDA_IN();		//设置SDA为输入 ,对的
	delay_us(500);
	SCCB_SCL_1;			//接收第九位,以判断是否发送成功
	delay_us(500);
	if(SCCB_READ_SDA)res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	SCCB_SCL_0;		 
	SCCB_SDA_OUT();		//设置SDA为输出    
	return res;  
}	 
//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN();		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		delay_us(500);
		SCCB_SCL_1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		delay_us(500);
		SCCB_SCL_0;
	}	
	SCCB_SDA_OUT();		//设置SDA为输出    
	return temp;
} 							    
//写寄存器
//返回值:0,成功;1,失败.
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
  	SCCB_Stop();	  
  	return	res;
}		  					    
//读寄存器
//返回值:读到的寄存器值
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//启动SCCB传输
	if(0==SCCB_WR_Byte(0x42))
	{
			SCCB_Stop();
  		delay_us(500);
	}  
	 if(0==SCCB_WR_Byte(reg))
	 {
	 	  SCCB_Stop();  
	    delay_us(500);	 
	 }
    SCCB_Stop();			  
	 delay_us(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	if(0==SCCB_WR_Byte(SCCB_ID|0X01))	//发送读命令
	{	  
		 SCCB_Stop();
	   delay_us(100);	 
	}
  	val=SCCB_RD_Byte();		 	//读取数据
		delay_us(500);
  	SCCB_No_Ack();
		delay_us(500);
  	SCCB_Stop();
  	return val;
}















