#include "ctiic.h"
#include "SysTick.h"



void CT_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP; //���ģʽ
	GPIO_InitStructure.Pin=T_MOSI_Pin;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;//�ٶ�Ϊ100M
	GPIO_InitStructure.Pull=GPIO_PULLUP;//����
	HAL_GPIO_Init(GPIOF,&GPIO_InitStructure); //��ʼ���ṹ��
}

void CT_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT; //����ģʽ
	GPIO_InitStructure.Pin=T_MOSI_Pin;
	GPIO_InitStructure.Pull=GPIO_PULLUP;//����
	HAL_GPIO_Init(GPIOF,&GPIO_InitStructure); //��ʼ���ṹ��
}

//����I2C�ٶȵ���ʱ
void CT_Delay(void)
{
	delay_us(5);
}

//���ݴ���оƬIIC�ӿڳ�ʼ��
void CT_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
  __HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();//ʹ��GPIOB,Fʱ��
	
	GPIO_InitStructure.Pin = T_SCK_Pin;//PB0����Ϊ�������
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//���ģʽ
	GPIO_InitStructure.Pull = GPIO_PULLUP;//����
	HAL_GPIO_Init(T_SCK_GPIO_Port, &GPIO_InitStructure);//��ʼ��
		
	GPIO_InitStructure.Pin = T_MOSI_Pin;//PF11�����������
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//���ģʽ
	HAL_GPIO_Init(T_MOSI_GPIO_Port, &GPIO_InitStructure);//��ʼ��	
}

//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda�����
	CT_IIC_SDA_1;	  	  
	CT_IIC_SCL_1;
	delay_us(30);
 	CT_IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	CT_IIC_SCL_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda�����
	CT_IIC_SCL_1;
	delay_us(30);
	CT_IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	CT_IIC_SDA_1;//����I2C���߽����ź�  
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA����Ϊ����  
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
	CT_IIC_SCL_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
  CT_IIC_SCL_0;//����ʱ�ӿ�ʼ���ݴ���
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
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CT_SDA_IN();//SDA����Ϊ����
	delay_us(30);
	for(i=0;i<8;i++ )
	{ 
		CT_IIC_SCL_0; 	    	   
		CT_Delay();
		CT_IIC_SCL_1;	 
		receive<<=1;
		if(CT_IIC_SDA_READ)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//����nACK
	else CT_IIC_Ack(); //����ACK   
 	return receive;
}

