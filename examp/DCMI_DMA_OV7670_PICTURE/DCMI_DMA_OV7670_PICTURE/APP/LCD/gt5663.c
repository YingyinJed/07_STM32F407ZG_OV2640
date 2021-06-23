#include "gt5663.h"
#include "touch.h"
#include "ctiic.h"
#include "SysTick.h" 
#include "string.h" 





//GT5663���ò�����
//��һ���ֽ�Ϊ�汾��(0X60),���뱣֤�µİ汾�Ŵ��ڵ���GT5663�ڲ�
//flashԭ�а汾��,�Ż��������.
const u8 GT5663_CFG_TBL[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
}; 


//����GT5663���ò���
//mode:0,���������浽flash
//     1,�������浽flash
u8 GT5663_Send_Cfg(u8 mode)
{
	u8 buf[2];
	u8 i=0;
	buf[0]=0;
	buf[1]=mode;	//�Ƿ�д�뵽GT5663 FLASH?  ���Ƿ���籣��
	for(i=0;i<sizeof(GT5663_CFG_TBL);i++)buf[0]+=GT5663_CFG_TBL[i];//����У���
    buf[0]=(~buf[0])+1;
	GT5663_WR_Reg(GT_CFGS_REG,(u8*)GT5663_CFG_TBL,sizeof(GT5663_CFG_TBL));//���ͼĴ�������
	GT5663_WR_Reg(GT_CHECK_REG,buf,2);//д��У���,�����ø��±��
	return 0;
} 

//��GT5663д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 GT5663_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//����д���� 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}

//��GT5663����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void GT5663_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //����д���� 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����    
} 

//��ʼ��GT5663������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
u8 GT5663_Init(void)
{
	u8 temp[5]; 
	GPIO_InitTypeDef  GPIO_InitStructure;	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();//ʹ��GPIOB,Cʱ��

	GPIO_InitStructure.Pin = T_PEN_Pin ;//PB1����Ϊ��������
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//����ģʽ
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;//100MHz
	GPIO_InitStructure.Pull = GPIO_PULLUP;//����
	HAL_GPIO_Init(T_PEN_GPIO_Port, &GPIO_InitStructure);//��ʼ��
		
	GPIO_InitStructure.Pin = T_CS_Pin;//PC13����Ϊ�������
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//���ģʽ
	HAL_GPIO_Init(T_CS_GPIO_Port, &GPIO_InitStructure);//��ʼ��	
	
	CT_IIC_Init();      	//��ʼ����������I2C����  
	GT_RST_0;				//��λ
	delay_ms(10);
 	GT_RST_1;				//�ͷŸ�λ		    
	delay_ms(10); 
	
	GPIO_InitStructure.Pin = T_PEN_Pin;//PB1����Ϊ��������
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//���ģʽ
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(T_PEN_GPIO_Port, &GPIO_InitStructure);//��ʼ��	
	
	delay_ms(100);  
	GT5663_RD_Reg(GT_PID_REG,temp,4);//��ȡ��ƷID
	temp[4]=0;
	if(strcmp((char*)temp,"5663")==0)//ID==5663
	{
		temp[0]=0X02;			
		GT5663_WR_Reg(GT_CTRL_REG,temp,1);//��λGT5663
		GT5663_RD_Reg(GT_CFGS_REG,temp,1);//��ȡGT_CFGS_REG�Ĵ���
//		if(temp[0]<0X60)//Ĭ�ϰ汾�Ƚϵ�,��Ҫ����flash����
//		{
//			printf("Default Ver:%d\r\n",temp[0]);
//			GT5663_Send_Cfg(1);//���²���������
//		}
		delay_ms(10);
		temp[0]=0X00;	 
		GT5663_WR_Reg(GT_CTRL_REG,temp,1);//������λ   
		return 0;
	} 
	return 1;
}

const u16 GT5663_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};
//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//1,�����޴���;0,�����д���
u8 GT5663_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=1;
	u8 temp;
 	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{
		GT5663_RD_Reg(GT_GSTID_REG,&mode,1);	//��ȡ�������״̬ 
		//printf("mode=%d\r\n",mode);		
 		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT5663_WR_Reg(GT_GSTID_REG,&temp,1);//���־ 		
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			TouchData.tpsta=(~temp); 
			TouchData.lcdx[4]=TouchData.lcdx[0];	//���津��0������
			TouchData.lcdy[4]=TouchData.lcdy[0];
			for(i=0;i<5;i++)
			{
				if(TouchData.tpsta&(1<<i))	//������Ч?
				{
					GT5663_RD_Reg(GT5663_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
					if(tftlcd_data.dir==1)//����
					{
						TouchData.lcdy[i]=((u16)buf[1]<<8)+buf[0];
						TouchData.lcdx[i]=800-(((u16)buf[3]<<8)+buf[2]);
					}else
					{
						TouchData.lcdx[i]=((u16)buf[1]<<8)+buf[0];
						TouchData.lcdy[i]=((u16)buf[3]<<8)+buf[2];
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,TouchData.lcdx[i],i,TouchData.lcdy[i]);
				}			
			} 
			res=0;
			if(TouchData.lcdx[0]>tftlcd_data.width||TouchData.lcdy[0]>tftlcd_data.height)//�Ƿ�����(���곬����)
			{ 
				if((mode&0XF)>1)		//��������������,�򸴵ڶ�����������ݵ���һ������.
				{
					TouchData.lcdx[0]=TouchData.lcdx[1];
					TouchData.lcdy[0]=TouchData.lcdy[1];
					t=0;				//����һ��,��������������10��,�Ӷ����������
				}else					//�Ƿ�����,����Դ˴�����(��ԭԭ����)  
				{
					TouchData.lcdx[0]=TouchData.lcdx[4];
					TouchData.lcdy[0]=TouchData.lcdy[4];
					mode=0X80;		
				}
			}else t=0;					//����һ��,��������������10��,�Ӷ����������
		}
	}
	if((mode&0X8F)==0X80)//�޴����㰴��
	{ 
		TouchData.tpsta=0;
		TouchData.lcdx[0]=0xffff;
		TouchData.lcdy[0]=0xffff;	 
	} 	
	if(t>240)t=10;//���´�10��ʼ����
	return res;
}
