#include "I2C_S.h"
#include "SysTick.h"
/*******************************************************************************
* 函 数 名         : IIC_Init
* 函数功能		   : IIC初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	//GPIOB8,B9初始化设置
	GPIO_InitStructure.Pin = I2C1_SCL_Pin|I2C1_SDA_Pin;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	IIC_SCL_1;
	IIC_SDA_1;	
}

/*******************************************************************************
* 函 数 名         : SDA_OUT
* 函数功能		   : SDA输出配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//GPIOB9初始化设置
	GPIO_InitStructure.Pin =I2C1_SDA_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//普通输出模式
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;//100MHz
	GPIO_InitStructure.Pull = GPIO_PULLUP;//上拉
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
}

/*******************************************************************************
* 函 数 名         : SDA_IN
* 函数功能		   : SDA输入配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//GPIOB9初始化设置
	GPIO_InitStructure.Pin =I2C1_SDA_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//输入模式
	GPIO_InitStructure.Pull = GPIO_PULLUP;//上拉
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
}

/*******************************************************************************
* 函 数 名         : IIC_Start
* 函数功能		   : 产生IIC起始信号   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_1;	  	  
	IIC_SCL_1;
	delay_us(5);
 	IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	delay_us(6);
	IIC_SCL_0;//钳住I2C总线，准备发送或接收数据 
}	

/*******************************************************************************
* 函 数 名         : IIC_Stop
* 函数功能		   : 产生IIC停止信号   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_0;
	IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
 	IIC_SCL_1; 
	delay_us(6); 
	IIC_SDA_1;//发送I2C总线结束信号
	delay_us(6);							   	
}

/*******************************************************************************
* 函 数 名         : IIC_Wait_Ack
* 函数功能		   : 等待应答信号到来   
* 输    入         : 无
* 输    出         : 1，接收应答失败
        			 0，接收应答成功
*******************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 tempTime=0; 
	IIC_SDA_1;
	delay_us(1);	   
	IIC_SCL_1;
	delay_us(1);
  SDA_IN();      //SDA设置为输入 
	while(IIC_SDA_READ)
	{
		tempTime++;
		if(tempTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0;//时钟输出0 	   
	return 0;  
} 

/*******************************************************************************
* 函 数 名         : IIC_Ack
* 函数功能		   : 产生ACK应答  
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_0;
	delay_us(2);
	IIC_SCL_1;
	delay_us(5);
	IIC_SCL_0;
}

/*******************************************************************************
* 函 数 名         : IIC_NAck
* 函数功能		   : 产生NACK非应答  
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/		    
void IIC_NAck(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_1;
	delay_us(2);
	IIC_SCL_1;
	delay_us(5);
	IIC_SCL_0;
}	

/*******************************************************************************
* 函 数 名         : IIC_Send_Byte
* 函数功能		   : IIC发送一个字节 
* 输    入         : txd：发送一个字节
* 输    出         : 无
*******************************************************************************/		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>0) //0x80  1000 0000
			IIC_SDA_1;
		else
			IIC_SDA_0;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_1;
		delay_us(2); 
		IIC_SCL_0;	
		delay_us(2);
    }	 
} 

/*******************************************************************************
* 函 数 名         : IIC_Read_Byte
* 函数功能		   : IIC读一个字节 
* 输    入         : ack=1时，发送ACK，ack=0，发送nACK 
* 输    出         : 应答或非应答
*******************************************************************************/  
u8 IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_0; 
        delay_us(2);
		IIC_SCL_1;
        receive<<=1;
        if(IIC_SDA_READ)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}






