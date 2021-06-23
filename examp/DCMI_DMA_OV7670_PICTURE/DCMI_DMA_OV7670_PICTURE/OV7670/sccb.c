#include "sccb.h"
#include "SysTick.h"
//��ʼ��SCCB�ӿ� 
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

//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
    SCCB_SDA_1;     //�����߸ߵ�ƽ	 
    delay_us(500); 
    SCCB_SCL_1;	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(500);  
    SCCB_SDA_0;
    delay_us(500);	 
    SCCB_SCL_0;	    //�����߻ָ��͵�ƽ��������������Ҫ	
    delay_us(500);   
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
    SCCB_SDA_0;
    delay_us(500);	 
    SCCB_SCL_1;	
    delay_us(500); 
    SCCB_SDA_1;	
    delay_us(500);
}  
//����NA�ź�
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
//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80)SCCB_SDA_1;	
		else SCCB_SDA_0;
		dat<<=1;
		delay_us(500);
		SCCB_SCL_1;	
		delay_us(500);
		SCCB_SCL_0;		   
	}			 
	SCCB_SDA_IN();		//����SDAΪ���� ,�Ե�
	delay_us(500);
	SCCB_SCL_1;			//���յھ�λ,���ж��Ƿ��ͳɹ�
	delay_us(500);
	if(SCCB_READ_SDA)res=1;  //SDA=1����ʧ�ܣ�����1
	else res=0;         //SDA=0���ͳɹ�������0
	SCCB_SCL_0;		 
	SCCB_SDA_OUT();		//����SDAΪ���    
	return res;  
}	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN();		//����SDAΪ����  
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		delay_us(500);
		SCCB_SCL_1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		delay_us(500);
		SCCB_SCL_0;
	}	
	SCCB_SDA_OUT();		//����SDAΪ���    
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
	delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//д����	 
  	SCCB_Stop();	  
  	return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//����SCCB����
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
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	if(0==SCCB_WR_Byte(SCCB_ID|0X01))	//���Ͷ�����
	{	  
		 SCCB_Stop();
	   delay_us(100);	 
	}
  	val=SCCB_RD_Byte();		 	//��ȡ����
		delay_us(500);
  	SCCB_No_Ack();
		delay_us(500);
  	SCCB_Stop();
  	return val;
}















