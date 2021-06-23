#include "ctiic.h"
#include "SysTick.h"



void CT_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP; //输出模式
	GPIO_InitStructure.Pin=T_MOSI_Pin;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;//速度为100M
	GPIO_InitStructure.Pull=GPIO_PULLUP;//上拉
	HAL_GPIO_Init(GPIOF,&GPIO_InitStructure); //初始化结构体
}

void CT_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT; //输入模式
	GPIO_InitStructure.Pin=T_MOSI_Pin;
	GPIO_InitStructure.Pull=GPIO_PULLUP;//上拉
	HAL_GPIO_Init(GPIOF,&GPIO_InitStructure); //初始化结构体
}

//控制I2C速度的延时
void CT_Delay(void)
{
	delay_us(5);
}

//电容触摸芯片IIC接口初始化
void CT_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
  __HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();//使能GPIOB,F时钟
	
	GPIO_InitStructure.Pin = T_SCK_Pin;//PB0设置为推挽输出
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//输出模式
	GPIO_InitStructure.Pull = GPIO_PULLUP;//上拉
	HAL_GPIO_Init(T_SCK_GPIO_Port, &GPIO_InitStructure);//初始化
		
	GPIO_InitStructure.Pin = T_MOSI_Pin;//PF11设置推挽输出
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//输出模式
	HAL_GPIO_Init(T_MOSI_GPIO_Port, &GPIO_InitStructure);//初始化	
}

//产生IIC起始信号
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda线输出
	CT_IIC_SDA_1;	  	  
	CT_IIC_SCL_1;
	delay_us(30);
 	CT_IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	CT_IIC_SCL_0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda线输出
	CT_IIC_SCL_1;
	delay_us(30);
	CT_IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	CT_IIC_SDA_1;//发送I2C总线结束信号  
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA设置为输入  
	CT_IIC_SDA_1;	   
	CT_IIC_SCL_1;
	CT_Delay();
	while(CT_IIC_SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		} 
		CT_Delay();
	}
	CT_IIC_SCL_0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void CT_IIC_Ack(void)
{
	CT_IIC_SCL_0;
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA_0;
	CT_Delay();
	CT_IIC_SCL_1;
	CT_Delay();
	CT_IIC_SCL_0;
}
//不产生ACK应答		    
void CT_IIC_NAck(void)
{
	CT_IIC_SCL_0;
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA_1;
	CT_Delay();
	CT_IIC_SCL_1;
	CT_Delay();
	CT_IIC_SCL_0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
  CT_IIC_SCL_0;//拉低时钟开始数据传输
	CT_Delay();
	for(t=0;t<8;t++)
  { 
      if((txd&0x80)>0) CT_IIC_SDA_1;
      else CT_IIC_SDA_0;
      txd<<=1; 	      
      CT_IIC_SCL_1; 
      CT_Delay();
      CT_IIC_SCL_0;	
      CT_Delay();
  }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CT_SDA_IN();//SDA设置为输入
	delay_us(30);
	for(i=0;i<8;i++ )
	{ 
		CT_IIC_SCL_0; 	    	   
		CT_Delay();
		CT_IIC_SCL_1;	 
		receive<<=1;
		if(CT_IIC_SDA_READ)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//发送nACK
	else CT_IIC_Ack(); //发送ACK   
 	return receive;
}

