#include "I2C_S.h"
#include "SysTick.h"
/*******************************************************************************
* �� �� ��         : IIC_Init
* ��������		   : IIC��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	//GPIOB8,B9��ʼ������
	GPIO_InitStructure.Pin = I2C1_SCL_Pin|I2C1_SDA_Pin;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	IIC_SCL_1;
	IIC_SDA_1;	
}

/*******************************************************************************
* �� �� ��         : SDA_OUT
* ��������		   : SDA�������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//GPIOB9��ʼ������
	GPIO_InitStructure.Pin =I2C1_SDA_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//��ͨ���ģʽ
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;//100MHz
	GPIO_InitStructure.Pull = GPIO_PULLUP;//����
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
}

/*******************************************************************************
* �� �� ��         : SDA_IN
* ��������		   : SDA��������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//GPIOB9��ʼ������
	GPIO_InitStructure.Pin =I2C1_SDA_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//����ģʽ
	GPIO_InitStructure.Pull = GPIO_PULLUP;//����
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
}

/*******************************************************************************
* �� �� ��         : IIC_Start
* ��������		   : ����IIC��ʼ�ź�   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA_1;	  	  
	IIC_SCL_1;
	delay_us(5);
 	IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	delay_us(6);
	IIC_SCL_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	

/*******************************************************************************
* �� �� ��         : IIC_Stop
* ��������		   : ����IICֹͣ�ź�   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_0;
	IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
 	IIC_SCL_1; 
	delay_us(6); 
	IIC_SDA_1;//����I2C���߽����ź�
	delay_us(6);							   	
}

/*******************************************************************************
* �� �� ��         : IIC_Wait_Ack
* ��������		   : �ȴ�Ӧ���źŵ���   
* ��    ��         : ��
* ��    ��         : 1������Ӧ��ʧ��
        			 0������Ӧ��ɹ�
*******************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 tempTime=0; 
	IIC_SDA_1;
	delay_us(1);	   
	IIC_SCL_1;
	delay_us(1);
  SDA_IN();      //SDA����Ϊ���� 
	while(IIC_SDA_READ)
	{
		tempTime++;
		if(tempTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0;//ʱ�����0 	   
	return 0;  
} 

/*******************************************************************************
* �� �� ��         : IIC_Ack
* ��������		   : ����ACKӦ��  
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : IIC_NAck
* ��������		   : ����NACK��Ӧ��  
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : IIC_Send_Byte
* ��������		   : IIC����һ���ֽ� 
* ��    ��         : txd������һ���ֽ�
* ��    ��         : ��
*******************************************************************************/		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>0) //0x80  1000 0000
			IIC_SDA_1;
		else
			IIC_SDA_0;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_1;
		delay_us(2); 
		IIC_SCL_0;	
		delay_us(2);
    }	 
} 

/*******************************************************************************
* �� �� ��         : IIC_Read_Byte
* ��������		   : IIC��һ���ֽ� 
* ��    ��         : ack=1ʱ������ACK��ack=0������nACK 
* ��    ��         : Ӧ����Ӧ��
*******************************************************************************/  
u8 IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}






