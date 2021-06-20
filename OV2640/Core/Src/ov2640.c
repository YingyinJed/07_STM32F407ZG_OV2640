//Ӱ����(ZGT)
#include <ov2640.h>
#include "tim.h"//���붨ʱ����ʱ
#include "ov2640cfg.h"
#include "stdio.h"

/*------------------------SCCB�ײ㺯��(��ʼ)------------------------*/
void SCCB_Delay_us(unsigned int us)
{
    uint16_t differ=0xffff-us-5;     
    HAL_TIM_Base_Start(&htim7);
    __HAL_TIM_SetCounter(&htim7,differ); 
    while(differ < 0xffff-5) 
    { 
        differ = __HAL_TIM_GetCounter(&htim7); 
    } 
    HAL_TIM_Base_Stop(&htim7);
}
/*******************************************************************************
* �� �� ��       :SCCB_Start()
* ��������       :SCCB������ʼ�ź�
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/
void SCCB_Start(void)
{
    SCCB_SDA_1;     //�����߸ߵ�ƽ	 
    SCCB_SCL_1;	  //��ʱ���߸ߵ�ʱ���������ɸ�����
    SCCB_Delay_us(50);  
    SCCB_SDA_0;
    SCCB_Delay_us(50); 
    SCCB_SCL_0;	    //�����߻ָ��͵�ƽ��������������Ҫ       
}

/*******************************************************************************
* �� �� ��       :SCCB_Stop()
* ��������       :SCCB����ֹͣ�ź�
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/
void SCCB_Stop(void)
{
    SCCB_SDA_0;
    SCCB_Delay_us(50);	 
    SCCB_SCL_1;	
    SCCB_Delay_us(50); 
    SCCB_SDA_1;	
    SCCB_Delay_us(50);
}  

/*******************************************************************************
* �� �� ��       :SCCB_No_Ack()
* ��������       :SCCB����NA�źţ���Ӧ���ź�
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/
void SCCB_No_Ack(void)
{
	SCCB_Delay_us(50);
	SCCB_SDA_1;	
	SCCB_SCL_1;	
	SCCB_Delay_us(50);
	SCCB_SCL_0;	
	SCCB_Delay_us(50);
	SCCB_SDA_0;	
	SCCB_Delay_us(50);
}
/*******************************************************************************
* �� �� ��       :SCCB_WR_Byte()
* ��������       :SCCBдһ���ֽ�����
* ��    ��       : dat����Ҫд�������
* ��    ��       : 0�ɹ���1ʧ��
*******************************************************************************/
unsigned char SCCB_WR_Byte(unsigned char dat)
{
	unsigned char j,res;	 
	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80)SCCB_SDA_1;	
		else SCCB_SDA_0;
		dat<<=1;
		SCCB_Delay_us(50);
		SCCB_SCL_1;	
		SCCB_Delay_us(50);
		SCCB_SCL_0;		   
	}
    SCCB_SDA_IN();		//����SDAΪ����  
	SCCB_Delay_us(50);
	SCCB_SCL_1;			//���յھ�λ,���ж��Ƿ��ͳɹ�
	SCCB_Delay_us(50);
	if(SCCB_SDA_READ())res=1;  //SDA=1����ʧ�ܣ�����1
	else res=0;         //SDA=0���ͳɹ�������0
	SCCB_SCL_0;
    SCCB_SDA_OUT();		//����SDAΪ���     
	return res;  
}	
/*******************************************************************************
* �� �� ��       :SCCB_RD_Byte()
* ��������       :SCCB��ȡһ���ֽڵ��ź�
* ��    ��       : ��
* ��    ��       : ����������
*******************************************************************************/
unsigned char SCCB_RD_Byte(void)
{
	unsigned char temp=0,j;    
    SCCB_SDA_IN();		//����SDAΪ����
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		SCCB_Delay_us(50);
		SCCB_SCL_1;
		temp=temp<<1;
		if(SCCB_SDA_READ())temp++;   
		SCCB_Delay_us(50);
		SCCB_SCL_0;
	}
    SCCB_SDA_OUT();		//����SDAΪ��� 	
	return temp;
} 
/*------------------------SCCB�ײ㺯��(����)------------------------*/

/*------------------------SCCBӦ�ú���(��ʼ)------------------------*/
/*******************************************************************************
* �� �� ��       :SCCB_WR_Reg()
* ��������       :SCCBд��һ���ֽڵ����ݵ��Ĵ���
* ��    ��       : reg���Ĵ�����ַ��     data��д�������
* ��    ��       : �ɹ�����0��ʧ�ܷ���1
*******************************************************************************/
unsigned char SCCB_WR_Reg(unsigned char reg,unsigned char data)
{
	unsigned char res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
	SCCB_Delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	SCCB_Delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//д����	 
  	SCCB_Stop();	  
  	return	res;
}
/*******************************************************************************
* �� �� ��       :SCCB_RD_Reg()
* ��������       :SCCB�ӼĴ�����ȡһ���ֽڵ�����
* ��    ��       : reg���Ĵ�����ַ
* ��    ��       : ���ض�ȡ��������
*******************************************************************************/
unsigned char SCCB_RD_Reg(unsigned char reg)
{
	unsigned char val=0;
    //����Ҫ���ļĴ���
	SCCB_Start();           //����SCCB����
    SCCB_WR_Byte(SCCB_ID);  //ȷ����ͨ��·����Ҫ��ϵ���豸ID
  	SCCB_Delay_us(100);	  
	SCCB_WR_Byte(reg);//ȷ����Ҫ���ļĴ���
    SCCB_Delay_us(100);		
    SCCB_Stop();			  
	SCCB_Delay_us(100);	
    //��ʼ���Ĵ�������
    SCCB_Start(); 
	SCCB_WR_Byte(SCCB_ID|0X01);	//���Ͷ�����	
	SCCB_Delay_us(100);	 	
  	val=SCCB_RD_Byte();		 	//��ȡ����	
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}
/*******************************************************************************
* �� �� ��       :SCCB_Init()
* ��������       :SCCB��ʼ��
* ��    ��       :��
* ��    ��       :��
*******************************************************************************/
void SCCB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOD_CLK_ENABLE();//����GPIOD���ʱ�ӣ���ΪSCCB���������Ƕ���ΪPD6��PD7
    GPIO_InitStruct.Pin = DCMI_SCL_Pin|DCMI_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DCMI_SCL_GPIO_Port,DCMI_SCL_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(DCMI_SDA_GPIO_Port,DCMI_SDA_Pin,GPIO_PIN_SET);
    SCCB_SDA_OUT();
}
/*------------------------SCCBӦ�ú���(����)------------------------*/


/*------------------------OV2640Ӧ�ú���(��ʼ)------------------------*/
/*******************************************************************************
* �� �� ��       :OV2640_Init()
* ��������       :��ʼ��OV2640��Ĭ�ϳ�ʼ�������1600*1200ͼƬ
* ��    ��       : reg���Ĵ�����ַ
* ��    ��       : ���ض�ȡ��������
*******************************************************************************/
uint8_t OV2640_Init(void)
{ 
	unsigned short int i=0;
    unsigned short int reg_ID=0;
//    unsigned int reg;
    //�����ϵ�\��λ����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOG_CLK_ENABLE();
    GPIO_InitStruct.Pin = OV2640_PWDN_Pin|OV2640_RESET_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    //Ӳ����λ
 	OV2640_PWDN_0;	//POWER ON
	HAL_Delay(10);
	OV2640_RST_0;	//��λOV2640
	HAL_Delay(10);
	OV2640_RST_1;	//������λ  
    //��ʼͨ��SCCB����OV2640
    SCCB_Init();    //��ʼ��SCCB ��IO��
	SCCB_WR_Reg(OV2640_DSP_RA_DLMT, 0x01);	//����sensor�Ĵ���
 	SCCB_WR_Reg(OV2640_SENSOR_COM7, 0x80);	//��λOV2640
	HAL_Delay(50);     

    reg_ID=SCCB_RD_Reg(OV2640_SENSOR_MIDH);	//��ȡ����ID �߰�λ07FA2
	reg_ID<<=8;
	reg_ID|=SCCB_RD_Reg(OV2640_SENSOR_MIDL);	//��ȡ����ID �Ͱ�λ
	if(reg_ID!=OV2640_MID)
	{
		
		return 1;
	}
    printf("MID:%d\r\n",reg_ID);
	reg_ID=SCCB_RD_Reg(OV2640_SENSOR_PIDH);	//��ȡ����ID �߰�λ
	reg_ID<<=8;
	reg_ID|=SCCB_RD_Reg(OV2640_SENSOR_PIDL);	//��ȡ����ID �Ͱ�λ
	if(reg_ID!=OV2640_PID)
	{
		
		return 2;
	} 
    printf("HID:%d\r\n",reg_ID);
 	//��ʼ�� OV2640,����SXGA�ֱ���(1600*1200)  
	for(i=0;i<sizeof(ov2640_sxga_init_reg_tbl)/2;i++)
	{
	   	SCCB_WR_Reg(ov2640_sxga_init_reg_tbl[i][0],ov2640_sxga_init_reg_tbl[i][1]);
 	} 
  	return 0x00; 	//ok
}
/*******************************************************************************
* �� �� ��       :OV2640_JPEG_Mode()
* ��������       :OV2640�л�JPEGģʽ
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/
void OV2640_JPEG_Mode(void) 
{
	uint16_t i=0;
	//����:YUV422��ʽ
	for(i=0;i<(sizeof(ov2640_yuv422_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_yuv422_reg_tbl[i][0],ov2640_yuv422_reg_tbl[i][1]); 
	} 
	
	//����:���JPEG����
	for(i=0;i<(sizeof(ov2640_jpeg_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_jpeg_reg_tbl[i][0],ov2640_jpeg_reg_tbl[i][1]);  
	}  
}
/*******************************************************************************
* �� �� ��       :OV2640_RGB565_Mode()
* ��������       :OV2640�л�REB565ģʽ
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/
void OV2640_RGB565_Mode(void) 
{
	uint16_t i=0;
	//����:RGB565���
	for(i=0;i<(sizeof(ov2640_rgb565_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_rgb565_reg_tbl[i][0],ov2640_rgb565_reg_tbl[i][1]); 
	} 
} 
//�Զ��ع����ò�����,֧��5���ȼ�
const static uint8_t OV2640_AUTOEXPOSURE_LEVEL[5][8]=
{
	{
		0xFF,0x01,
		0x24,0x20,
		0x25,0x18,
		0x26,0x60,
	},
	{
		0xFF,0x01,
		0x24,0x34,
		0x25,0x1c,
		0x26,0x00,
	},
	{
		0xFF,0x01,	
		0x24,0x3e,	
		0x25,0x38,
		0x26,0x81,
	},
	{
		0xFF,0x01,
		0x24,0x48,
		0x25,0x40,
		0x26,0x81,
	},
	{
		0xFF,0x01,	
		0x24,0x58,	
		0x25,0x50,	
		0x26,0x92,	
	},
}; 
/*******************************************************************************
* �� �� ��       :OV2640_Auto_Exposure(unsigned char level)
* ��������       :OV2640�Զ��ع�ȼ�����
* ��    ��       :�ع�ȼ�0~4
* ��    ��       : ��
*******************************************************************************/
void OV2640_Auto_Exposure(unsigned char level)
{  
	unsigned char i;
	unsigned char *p=(unsigned char*)OV2640_AUTOEXPOSURE_LEVEL[level];
	for(i=0;i<4;i++)
	{ 
		SCCB_WR_Reg(p[i*2],p[i*2+1]); 
	} 
}
/*******************************************************************************
* �� �� ��       :OV2640_Light_Mode(unsigned char mode)
* ��������       :��ƽ������
* ��    ��       :0-�Զ���1-�Զ���2-̫����3-���죻4-����
* ��    ��       : ��
*******************************************************************************/
void OV2640_Light_Mode(unsigned char mode)
{
	unsigned char regccval=0X5E;//Sunny 
	unsigned char regcdval=0X41;
	unsigned char regceval=0X54;
	switch(mode)
	{ 
		case 0://auto 
			SCCB_WR_Reg(0XFF,0X00);	 
			SCCB_WR_Reg(0XC7,0X10);//AWB ON 
			return;  	
		case 2://cloudy
			regccval=0X65;
			regcdval=0X41;
			regceval=0X4F;
			break;	
		case 3://office
			regccval=0X52;
			regcdval=0X41;
			regceval=0X66;
			break;	
		case 4://home
			regccval=0X42;
			regcdval=0X3F;
			regceval=0X71;
			break;	
	}
	SCCB_WR_Reg(0XFF,0X00);	 
	SCCB_WR_Reg(0XC7,0X40);	//AWB OFF 
	SCCB_WR_Reg(0XCC,regccval); 
	SCCB_WR_Reg(0XCD,regcdval); 
	SCCB_WR_Reg(0XCE,regceval);  
}
/*******************************************************************************
* �� �� ��       :OV2640_Color_Saturation(unsigned char sat)
* ��������       :ɫ������
* ��    ��       :0-(-2)��1-(-1)��2-(0)��3-(+1)��4-(+2)
* ��    ��       : ��
*******************************************************************************/
void OV2640_Color_Saturation(unsigned char sat)
{ 
	unsigned char reg7dval=((sat+2)<<4)|0X08;
	SCCB_WR_Reg(0XFF,0X00);		
	SCCB_WR_Reg(0X7C,0X00);		
	SCCB_WR_Reg(0X7D,0X02);				
	SCCB_WR_Reg(0X7C,0X03);			
	SCCB_WR_Reg(0X7D,reg7dval);			
	SCCB_WR_Reg(0X7D,reg7dval); 		
}
/*******************************************************************************
* �� �� ��       :OV2640_Brightness(unsigned char bright)
* ��������       :��������
* ��    ��       :0-(-2)��1-(-1)��2-(0)��3-(+1)��4-(+2)
* ��    ��       : ��
*******************************************************************************/
void OV2640_Brightness(unsigned char bright)
{
  SCCB_WR_Reg(0xff, 0x00);
  SCCB_WR_Reg(0x7c, 0x00);
  SCCB_WR_Reg(0x7d, 0x04);
  SCCB_WR_Reg(0x7c, 0x09);
  SCCB_WR_Reg(0x7d, bright<<4); 
  SCCB_WR_Reg(0x7d, 0x00); 
}
/*******************************************************************************
* �� �� ��       :OV2640_Contrast(unsigned char contrast)
* ��������       :�Աȶ�����
* ��    ��       :0-(-2)��1-(-1)��2-(0)��3-(+1)��4-(+2)
* ��    ��       : ��
*******************************************************************************/
void OV2640_Contrast(unsigned char contrast)
{
	unsigned char reg7d0val=0X20;//Ĭ��Ϊ��ͨģʽ
	unsigned char reg7d1val=0X20;
  	switch(contrast)
	{
		case 0://-2
			reg7d0val=0X18;	 	 
			reg7d1val=0X34;	 	 
			break;	
		case 1://-1
			reg7d0val=0X1C;	 	 
			reg7d1val=0X2A;	 	 
			break;	
		case 3://1
			reg7d0val=0X24;	 	 
			reg7d1val=0X16;	 	 
			break;	
		case 4://2
			reg7d0val=0X28;	 	 
			reg7d1val=0X0C;	 	 
			break;	
	}
	SCCB_WR_Reg(0xff,0x00);
	SCCB_WR_Reg(0x7c,0x00);
	SCCB_WR_Reg(0x7d,0x04);
	SCCB_WR_Reg(0x7c,0x07);
	SCCB_WR_Reg(0x7d,0x20);
	SCCB_WR_Reg(0x7d,reg7d0val);
	SCCB_WR_Reg(0x7d,reg7d1val);
	SCCB_WR_Reg(0x7d,0x06);
}
/*******************************************************************************
* �� �� ��       :OV2640_Special_Effects(unsigned char eft)
* ��������       :��Ч����
* ��    ��       :0��ͨģʽ��1��Ƭ��2�ڰף�3ƫ��ɫ��4ƫ��ɫ��5ƫ��ɫ��6����
* ��    ��       : ��
*******************************************************************************/   
void OV2640_Special_Effects(unsigned char eft)
{
	unsigned char reg7d0val=0X00;//Ĭ��Ϊ��ͨģʽ
	unsigned char reg7d1val=0X80;
	unsigned char reg7d2val=0X80; 
	switch(eft)
	{
		case 1://��Ƭ
			reg7d0val=0X40; 
			break;	
		case 2://�ڰ�
			reg7d0val=0X18; 
			break;	 
		case 3://ƫ��ɫ
			reg7d0val=0X18; 
			reg7d1val=0X40;
			reg7d2val=0XC0; 
			break;	
		case 4://ƫ��ɫ
			reg7d0val=0X18; 
			reg7d1val=0X40;
			reg7d2val=0X40; 
			break;	
		case 5://ƫ��ɫ
			reg7d0val=0X18; 
			reg7d1val=0XA0;
			reg7d2val=0X40; 
			break;	
		case 6://����
			reg7d0val=0X18; 
			reg7d1val=0X40;
			reg7d2val=0XA6; 
			break;	 
	}
	SCCB_WR_Reg(0xff,0x00);
	SCCB_WR_Reg(0x7c,0x00);
	SCCB_WR_Reg(0x7d,reg7d0val);
	SCCB_WR_Reg(0x7c,0x05);
	SCCB_WR_Reg(0x7d,reg7d1val);
	SCCB_WR_Reg(0x7d,reg7d2val); 
}
/*******************************************************************************
* �� �� ��       :OV2640_Color_Bar(unsigned char sw)
* ��������       :��������
* ��    ��       :0�رգ�1������
* ��    ��       : ��
*******************************************************************************/  
void OV2640_Color_Bar(unsigned char sw)
{
	unsigned char reg;
	SCCB_WR_Reg(0XFF,0X01);
	reg=SCCB_RD_Reg(0X12);
	reg&=~(1<<1);
	if(sw)reg|=1<<1; 
	SCCB_WR_Reg(0X12,reg);
}
/*******************************************************************************
* �� �� ��       :OV2640_Window_Set(unsigned int sx,unsigned int sy,unsigned int width,unsigned int height)
* ��������       :����ͼ���������
* ��    ��       :sx,sy,��ʼ��ַ��width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical)
* ��    ��       : ��
*******************************************************************************/ 
void OV2640_Window_Set(unsigned int sx,unsigned int sy,unsigned int width,unsigned int height)
{
	unsigned int endx;
	unsigned int endy;
	unsigned char temp; 
	endx=sx+width/2;	//V*2
 	endy=sy+height/2;
	
	SCCB_WR_Reg(0XFF,0X01);			
	temp=SCCB_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endy&0X03)<<2)|(sy&0X03);
	SCCB_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
	SCCB_WR_Reg(0X19,sy>>2);			//����Vref��start��8λ
	SCCB_WR_Reg(0X1A,endy>>2);			//����Vref��end�ĸ�8λ
	
	temp=SCCB_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
	temp&=0XC0;
	temp|=((endx&0X07)<<3)|(sx&0X07);
	SCCB_WR_Reg(0X32,temp);				//����Href��start��end�����3λ
	SCCB_WR_Reg(0X17,sx>>3);			//����Href��start��8λ
	SCCB_WR_Reg(0X18,endx>>3);			//����Href��end�ĸ�8λ
}
/*******************************************************************************
* �� �� ��       :OV2640_OutSize_Set(uint16_t width,uint16_t height)
* ��������       :����ͼ�������С
* ��    ��       :width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical),width��height������4�ı���
* ��    ��       : 0,���óɹ�������Ϊʧ��
*******************************************************************************/ 
unsigned char OV2640_OutSize_Set(uint16_t width,uint16_t height)
{
	unsigned int outh;
	unsigned int outw;
	unsigned char temp; 
	if(width%4)return 1;
	if(height%4)return 2;
	outw=width/4;
	outh=height/4; 
	SCCB_WR_Reg(0XFF,0X00);	
	SCCB_WR_Reg(0XE0,0X04);			
	SCCB_WR_Reg(0X5A,outw&0XFF);		//����OUTW�ĵͰ�λ
	SCCB_WR_Reg(0X5B,outh&0XFF);		//����OUTH�ĵͰ�λ
	temp=(outw>>8)&0X03;
	temp|=(outh>>6)&0X04;
	SCCB_WR_Reg(0X5C,temp);				//����OUTH/OUTW�ĸ�λ 
	SCCB_WR_Reg(0XE0,0X00);	
	return 0;
}

//����ͼ�񿪴���С
//��:OV2640_ImageSize_Setȷ������������ֱ��ʴӴ�С.
//�ú������������Χ������п���,����OV2640_OutSize_Set�����
//ע��:�������Ŀ�Ⱥ͸߶�,������ڵ���OV2640_OutSize_Set�����Ŀ�Ⱥ͸߶�
//     OV2640_OutSize_Set���õĿ�Ⱥ͸߶�,���ݱ��������õĿ�Ⱥ͸߶�,��DSP
//     �Զ��������ű���,������ⲿ�豸.
//width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical),width��height������4�ı���
//����ֵ:0,���óɹ�
//    ����,����ʧ��
/*******************************************************************************
* �� �� ��       :OV2640_ImageWin_Set(unsigned int offx,unsigned int offy,unsigned int width,unsigned int height)
* ��������       :����ͼ�������С
* ��    ��       :width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical),width��height������4�ı���
* ��    ��       : 0,���óɹ�������Ϊʧ��
*******************************************************************************/ 
unsigned char OV2640_ImageWin_Set(unsigned int offx,unsigned int offy,unsigned int width,unsigned int height)
{
	unsigned int hsize;
	unsigned int vsize;
	unsigned char temp; 
	if(width%4)return 1;
	if(height%4)return 2;
	hsize=width/4;
	vsize=height/4;
	SCCB_WR_Reg(0XFF,0X00);	
	SCCB_WR_Reg(0XE0,0X04);					
	SCCB_WR_Reg(0X51,hsize&0XFF);		//����H_SIZE�ĵͰ�λ
	SCCB_WR_Reg(0X52,vsize&0XFF);		//����V_SIZE�ĵͰ�λ
	SCCB_WR_Reg(0X53,offx&0XFF);		//����offx�ĵͰ�λ
	SCCB_WR_Reg(0X54,offy&0XFF);		//����offy�ĵͰ�λ
	temp=(vsize>>1)&0X80;
	temp|=(offy>>4)&0X70;
	temp|=(hsize>>5)&0X08;
	temp|=(offx>>8)&0X07; 
	SCCB_WR_Reg(0X55,temp);				//����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ
	SCCB_WR_Reg(0X57,(hsize>>2)&0X80);	//����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ
	SCCB_WR_Reg(0XE0,0X00);	
	return 0;
} 
//�ú�������ͼ��ߴ��С,Ҳ������ѡ��ʽ������ֱ���
//UXGA:1600*1200,SVGA:800*600,CIF:352*288
//width,height:ͼ���Ⱥ�ͼ��߶�
//����ֵ:0,���óɹ�
//    ����,����ʧ��
/*******************************************************************************
* �� �� ��       :OV2640_ImageSize_Set(unsigned int width,unsigned int height)
* ��������       :�ú�������ͼ��ߴ��С,Ҳ������ѡ��ʽ������ֱ���
* ��    ��       :width,height:ͼ���Ⱥ�ͼ��߶ȣ�UXGA:1600*1200,SVGA:800*600,CIF:352*288
* ��    ��       : 0,���óɹ�������Ϊʧ��
*******************************************************************************/ 
unsigned char OV2640_ImageSize_Set(unsigned int width,unsigned int height)
{ 
	unsigned char temp; 
	SCCB_WR_Reg(0XFF,0X00);			
	SCCB_WR_Reg(0XE0,0X04);			
	SCCB_WR_Reg(0XC0,(width)>>3&0XFF);		//����HSIZE��10:3λ
	SCCB_WR_Reg(0XC1,(height)>>3&0XFF);		//����VSIZE��10:3λ
	temp=(width&0X07)<<3;
	temp|=height&0X07;
	temp|=(width>>4)&0X80; 
	SCCB_WR_Reg(0X8C,temp);	
	SCCB_WR_Reg(0XE0,0X00);				 
	return 0;
}
/*------------------------OV2640Ӧ�ú���(����)------------------------*/
//DCMI DMA����
//mem0addr:�洢����ַ0  ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//mem1addr:�洢����ַ1  ��ֻʹ��mem0addr��ʱ��,��ֵ����Ϊ0
//memblen:�洢��λ��,����Ϊ:DMA_MDATAALIGN_BYTE/DMA_MDATAALIGN_HALFWORD/DMA_MDATAALIGN_WORD
//meminc:�洢��������ʽ,����Ϊ:DMA_MINC_ENABLE/DMA_MINC_DISABLE

//DCMI_HandleTypeDef hdcmi;
//DMA_HandleTypeDef hdma_dcmi;

