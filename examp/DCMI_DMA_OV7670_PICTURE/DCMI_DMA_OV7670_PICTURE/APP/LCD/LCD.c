#include "LCD.h"
#include "main.h"
#include "board.h"
#include "stdio.h"
#include "SysTick.h"
extern SRAM_HandleTypeDef hsram1;
//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 FRONT_COLOR=BLACK;	//������ɫ
u16 BACK_COLOR=WHITE;  //����ɫ 

_tftlcd_data tftlcd_data;

//д�Ĵ�������
//cmd:�Ĵ���ֵ
void LCD_WriteCmd(u16 cmd)
{	
#ifdef TFTLCD_NT35510		
	TFTLCD->LCD_CMD=cmd;	
#endif
}

//д����
//data:Ҫд���ֵ
void LCD_WriteData(u16 data)
{
#ifdef TFTLCD_NT35510		
	TFTLCD->LCD_DATA=data;	
#endif

}

void LCD_WriteCmdData(u16 cmd,u16 data)
{
	LCD_WriteCmd(cmd);
	LCD_WriteData(data);
}


//������
//����ֵ:������ֵ
u16 LCD_ReadData(void)
{
#ifdef TFTLCD_NT35510		
	return TFTLCD->LCD_DATA;
#endif
}

void LCD_WR_REG(vu16 regval)
{   
	TFTLCD->LCD_CMD=regval;//д��Ҫд�ļĴ������	 
}
void LCD_WR_DATA(vu16 data)
{	 
	TFTLCD->LCD_DATA=data;		 
}
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	TFTLCD->LCD_CMD = LCD_Reg;		//д��Ҫд�ļĴ������	 
	TFTLCD->LCD_DATA = LCD_RegValue;//д������	    		 
}	   

//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)  //Ĭ����������
	{		
#ifdef TFTLCD_NT35510	
		LCD_WriteCmd(0x3600);   
		LCD_WriteData(0x0000);
    tftlcd_data.wramcmd=0X2C00;
	 	tftlcd_data.setxcmd=0X2A00;
		tftlcd_data.setycmd=0X2B00; 
		tftlcd_data.height=800;
		tftlcd_data.width=480;
#endif
		tftlcd_data.dir=0;
		
	}
	else
	{	
#ifdef TFTLCD_NT35510	
		LCD_WriteCmd(0x3600);   
		LCD_WriteData(0x0060);
    tftlcd_data.wramcmd=0X2C00;
	 	tftlcd_data.setxcmd=0X2A00;
		tftlcd_data.setycmd=0X2B00; 
		tftlcd_data.height=480;
		tftlcd_data.width=800;
#endif
		tftlcd_data.dir=1;
	}	
}


void LCD_GPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct ={0};
  __HAL_RCC_FSMC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14 
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4 
                          |GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}
void LCD_FSMCInit(void)
{
  FSMC_NORSRAM_TimingTypeDef Timing = {0};
  FSMC_NORSRAM_TimingTypeDef ExtTiming = {0};

  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

  hsram1.Init.NSBank = FSMC_NORSRAM_BANK4;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  
  Timing.AddressSetupTime = 0X01;
  Timing.AddressHoldTime = 0X00;
  Timing.DataSetupTime = 0X0F;
  Timing.BusTurnAroundDuration = 0X00;
  Timing.CLKDivision = 0X00;
  Timing.DataLatency = 0X00;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;

  ExtTiming.AddressSetupTime = 0X03;
  ExtTiming.AddressHoldTime = 0X00;
  ExtTiming.DataSetupTime = 0X02;
  ExtTiming.BusTurnAroundDuration = 0X00;
  ExtTiming.CLKDivision = 0X00;
  ExtTiming.DataLatency = 0X00;
  ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;
  
  HAL_SRAM_Init(&hsram1, &Timing, &ExtTiming);

}
void TFTLCD_Init(void)
{
  LCD_GPIOInit();
  LCD_FSMCInit();
  delay_ms(50);
#ifdef TFTLCD_NT35510	
	LCD_WriteCmd(0XDA00);	
	tftlcd_data.id=LCD_ReadData();		//����0X00	 
	LCD_WriteCmd(0XDB00);	
	tftlcd_data.id=LCD_ReadData();		//����0X80
	tftlcd_data.id<<=8;	
	LCD_WriteCmd(0XDC00);	
	tftlcd_data.id|=LCD_ReadData();		//����0X00
#endif	
#ifdef TFTLCD_NT35510

	//************* Start Initial Sequence **********//
	//**********LV2 Page 1 enable***********//
	LCD_WriteCmd(0xF000);	LCD_WriteData(0x0055);
	LCD_WriteCmd(0xF001);	LCD_WriteData(0x00AA);
	LCD_WriteCmd(0xF002);	LCD_WriteData(0x0052);
	LCD_WriteCmd(0xF003);	LCD_WriteData(0x0008);
	LCD_WriteCmd(0xF004);	LCD_WriteData(0x0001);

	//*************AVDD Set AVDD 5.2V*************//
	LCD_WriteCmd(0xB000);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB001);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB002);	LCD_WriteData(0x0000);

	//************AVDD ratio****************//
	LCD_WriteCmd(0xB600);	LCD_WriteData(0x0034);
	LCD_WriteCmd(0xB601);	LCD_WriteData(0x0034);
	LCD_WriteCmd(0xB602);	LCD_WriteData(0x0034);
	 
	//************AVEE  -5.2V****************//
	LCD_WriteCmd(0xB100);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB101);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB102);	LCD_WriteData(0x0000);

	//***********AVEE ratio*************//
	LCD_WriteCmd(0xB700);	LCD_WriteData(0x0034);
	LCD_WriteCmd(0xB701);	LCD_WriteData(0x0034);
	LCD_WriteCmd(0xB702);	LCD_WriteData(0x0034);

	//***********VCL  -2.5V*************//
	LCD_WriteCmd(0xB200);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB201);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB202);	LCD_WriteData(0x0000);

	//**************VCL ratio*****************//
	LCD_WriteCmd(0xB800);	LCD_WriteData(0x0005);
	LCD_WriteCmd(0xB801);	LCD_WriteData(0x0005);
	LCD_WriteCmd(0xB802);	LCD_WriteData(0x0005);


	//*************VGH 15V  (Free pump)*********//
	LCD_WriteCmd(0xBF00);	LCD_WriteData(0x0001);
	LCD_WriteCmd(0xB300);	LCD_WriteData(0x000F);
	LCD_WriteCmd(0xB301);	LCD_WriteData(0x000F);
	LCD_WriteCmd(0xB302);	LCD_WriteData(0x000F);

	//*************VGH ratio*****************//
	LCD_WriteCmd(0xB900);	LCD_WriteData(0x0024);
	LCD_WriteCmd(0xB901);	LCD_WriteData(0x0024);
	LCD_WriteCmd(0xB902);	LCD_WriteData(0x0024);

	//***************VGL_REG -10V**************//
	LCD_WriteCmd(0xB500);	LCD_WriteData(0x0008);
	LCD_WriteCmd(0xB501);	LCD_WriteData(0x0008);
	LCD_WriteCmd(0xB502);	LCD_WriteData(0x0008);

//	LCD_WriteCmd(0xC2,0x00);	LCD_WriteData(0x00,0x03);
		 
	//*****************VGLX ratio***************//
	LCD_WriteCmd(0xBA00);	LCD_WriteData(0x0014);
	LCD_WriteCmd(0xBA01);	LCD_WriteData(0x0014);
	LCD_WriteCmd(0xBA02);	LCD_WriteData(0x0014);

	//*************VGMP/VGSP 4.5V/0V*************//
	LCD_WriteCmd(0xBC00);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xBC01);	LCD_WriteData(0x00BC);
	LCD_WriteCmd(0xBC02);	LCD_WriteData(0x0000);

	//************VGMN/VGSN -4.5V/0V****************//
	LCD_WriteCmd(0xBD00);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xBD01);	LCD_WriteData(0x00BC);
	LCD_WriteCmd(0xBD02);	LCD_WriteData(0x0000);

	//************VCOM  -1.25V****************//
	LCD_WriteCmd(0xBE00);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xBE01);	LCD_WriteData(0x002D);

	//************Gamma Setting******************//

	LCD_WriteCmd(0xD100); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD101); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD102); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD103); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD104); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD105); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD106); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD107); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD108); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD109); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD10A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD10B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD10C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD10D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD10E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD10F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD110); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD111); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD112); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD113); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD114); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD115); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD116); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD117); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD118); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD119); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD11A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD11B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD11C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD11D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD11E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD11F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD120); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD121); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD122); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD123); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD124); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD125); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD126); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD127); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD128); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD129); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD12A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD12B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD12C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD12D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD12E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD12F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD130); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD131); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD132); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD133); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD200); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD201); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD202); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD203); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD204); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD205); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD206); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD207); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD208); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD209); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD20A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD20B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD20C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD20D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD20E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD20F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD210); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD211); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD212); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD213); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD214); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD215); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD216); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD217); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD218); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD219); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD21A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD21B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD21C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD21D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD21E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD21F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD220); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD221); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD222); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD223); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD224); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD225); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD226); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD227); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD228); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD229); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD22A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD22B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD22C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD22D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD22E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD22F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD230); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD231); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD232); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD233); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD300); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD301); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD302); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD303); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD304); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD305); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD306); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD307); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD308); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD309); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD30A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD30B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD30C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD30D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD30E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD30F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD310); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD311); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD312); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD313); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD314); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD315); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD316); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD317); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD318); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD319); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD31A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD31B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD31C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD31D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD31E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD31F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD320); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD321); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD322); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD323); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD324); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD325); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD326); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD327); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD328); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD329); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD32A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD32B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD32C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD32D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD32E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD32F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD330); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD331); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD332); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD333); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD400); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD401); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD402); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD403); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD404); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD405); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD406); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD407); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD408); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD409); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD40A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD40B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD40C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD40D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD40E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD40F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD410); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD411); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD412); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD413); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD414); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD415); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD416); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD417); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD418); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD419); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD41A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD41B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD41C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD41D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD41E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD41F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD420); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD421); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD422); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD423); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD424); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD425); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD426); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD427); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD428); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD429); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD42A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD42B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD42C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD42D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD42E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD42F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD430); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD431); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD432); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD433); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD500); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD501); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD502); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD503); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD504); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD505); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD506); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD507); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD508); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD509); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD50A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD50B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD50C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD50D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD50E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD50F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD510); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD511); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD512); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD513); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD514); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD515); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD516); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD517); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD518); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD519); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD51A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD51B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD51C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD51D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD51E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD51F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD520); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD521); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD522); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD523); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD524); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD525); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD526); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD527); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD528); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD529); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD52A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD52B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD52C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD52D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD52E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD52F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD530); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD531); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD532); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD533); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD600); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD601); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD602); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD603); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD604); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD605); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD606); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD607); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD608); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD609); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD60A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD60B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD60C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD60D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD60E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD60F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD610); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD611); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD612); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD613); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD614); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD615); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD616); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD617); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD618); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD619); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD61A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD61B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD61C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD61D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD61E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD61F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD620); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD621); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD622); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD623); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD624); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD625); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD626); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD627); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD628); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD629); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD62A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD62B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD62C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD62D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD62E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD62F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD630); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD631); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD632); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD633); LCD_WriteData(0x00C1);


	//**************LV2 Page 0 enable*************//
	LCD_WriteCmd(0xF000);	LCD_WriteData(0x0055);
	LCD_WriteCmd(0xF001);	LCD_WriteData(0x00AA);
	LCD_WriteCmd(0xF002);	LCD_WriteData(0x0052);
	LCD_WriteCmd(0xF003);	LCD_WriteData(0x0008);
	LCD_WriteCmd(0xF004);	LCD_WriteData(0x0000);

	//*************480x800*********************//
	LCD_WriteCmd(0xB500);	LCD_WriteData(0x0050);

	//***************Display control**************//
	LCD_WriteCmd(0xB100);	LCD_WriteData(0x00CC);
	//LCD_WriteCmd(0xB101);	LCD_WriteData(0x0000);

	//***************Source hold time*************//
	LCD_WriteCmd(0xB600);	LCD_WriteData(0x0002);

	//**************Gate EQ control***************//
	//LCD_WriteCmd(0xB700);	LCD_WriteData(0x0070);
	//LCD_WriteCmd(0xB701);	LCD_WriteData(0x0070);

	//*************Source EQ control (Mode 2)******//
	LCD_WriteCmd(0xB800);	LCD_WriteData(0x0001);
	LCD_WriteCmd(0xB801);	LCD_WriteData(0x0003);
	LCD_WriteCmd(0xB802);	LCD_WriteData(0x0003);
	LCD_WriteCmd(0xB803);	LCD_WriteData(0x0003);
					
	//************Inversion mode  (2-dot)***********//
	LCD_WriteCmd(0xBC00);	LCD_WriteData(0x0005);
	LCD_WriteCmd(0xBC01);	LCD_WriteData(0x0005);
	LCD_WriteCmd(0xBC02);	LCD_WriteData(0x0005);
					 
	//***************Frame rate***************//      
	//LCD_WriteCmd(0xBD00);	LCD_WriteData(0x0001);
	//LCD_WriteCmd(0xBD01);	LCD_WriteData(0x0084);
	LCD_WriteCmd(0xBD02);	LCD_WriteData(0x0007);  //0X1C
	LCD_WriteCmd(0xBD03);	LCD_WriteData(0x0031);
	LCD_WriteCmd(0xBE02);	LCD_WriteData(0x0007);  
	LCD_WriteCmd(0xBE03);	LCD_WriteData(0x0031);
	LCD_WriteCmd(0xBF02);	LCD_WriteData(0x0007);
	LCD_WriteCmd(0xBE03);	LCD_WriteData(0x0031);
	
	
	LCD_WriteCmd(0xFF00);	LCD_WriteData(0x00AA);
	LCD_WriteCmd(0xFF01);	LCD_WriteData(0x0055);    
	LCD_WriteCmd(0xFF02);	LCD_WriteData(0x0025);
	LCD_WriteCmd(0xFF03);	LCD_WriteData(0x0001);
	
	LCD_WriteCmd(0xF306);	LCD_WriteData(0x0010);
	LCD_WriteCmd(0xF408);	LCD_WriteData(0x0000);
	

	//********Timing control 4H w/ 4-Delayms *******//
	/*
	LCD_WriteCmd(0xC900);	LCD_WriteData(0x00D0);
	LCD_WriteCmd(0xC901);	LCD_WriteData(0x0002); 
	LCD_WriteCmd(0xC902);	LCD_WriteData(0x0050);
	LCD_WriteCmd(0xC903);	LCD_WriteData(0x0050);
	LCD_WriteCmd(0xC904);	LCD_WriteData(0x0050);
	*/
	LCD_WriteCmd(0x3600);	LCD_WriteData(0x00C0);//����ɨ�跽ʽ��RGB  BGR
	LCD_WriteCmd(0x3500);	LCD_WriteData(0x0000);
	
	LCD_WriteCmd(0x3A00);	LCD_WriteData(0x0005);	//70
	 
	LCD_WriteCmd(0x1100);
	delay_ms(120);
	LCD_WriteCmd(0x2900);
	delay_ms(50);
#endif

  LCD_Display_Dir(TFTLCD_DIR);		//0������  1������  Ĭ������
	LCD_Clear(WHITE);
}



void LCD_WriteData_Color(u16 color)
{
#ifdef TFTLCD_NT35510
	TFTLCD->LCD_DATA=color;	
#endif
}
//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=tftlcd_data.width;
	totalpoint*=tftlcd_data.height; 			//�õ��ܵ���
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		TFTLCD->LCD_DATA=color;	
	}
}


////////////////////////////////////////////////////////////////////DCMI
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
 	TFTLCD->LCD_CMD=tftlcd_data.wramcmd;	  
}	 
//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height. 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;

  LCD_WR_REG(tftlcd_data.setxcmd);LCD_WR_DATA(sx>>8);  
  LCD_WR_REG(tftlcd_data.setxcmd+1);LCD_WR_DATA(sx&0XFF);	  
  LCD_WR_REG(tftlcd_data.setxcmd+2);LCD_WR_DATA(twidth>>8);   
  LCD_WR_REG(tftlcd_data.setxcmd+3);LCD_WR_DATA(twidth&0XFF);   
  LCD_WR_REG(tftlcd_data.setycmd);LCD_WR_DATA(sy>>8);   
  LCD_WR_REG(tftlcd_data.setycmd+1);LCD_WR_DATA(sy&0XFF);  
  LCD_WR_REG(tftlcd_data.setycmd+2);LCD_WR_DATA(theight>>8);   
  LCD_WR_REG(tftlcd_data.setycmd+3);LCD_WR_DATA(theight&0XFF);  

}


//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
  switch(dir)
  {
    case L2R_U2D://������,���ϵ���
      regval|=(0<<7)|(0<<6)|(0<<5); 
      break;
    case L2R_D2U://������,���µ���
      regval|=(1<<7)|(0<<6)|(0<<5); 
      break;
    case R2L_U2D://���ҵ���,���ϵ���
      regval|=(0<<7)|(1<<6)|(0<<5); 
      break;
    case R2L_D2U://���ҵ���,���µ���
      regval|=(1<<7)|(1<<6)|(0<<5); 
      break;	 
    case U2D_L2R://���ϵ���,������
      regval|=(0<<7)|(0<<6)|(1<<5); 
      break;
    case U2D_R2L://���ϵ���,���ҵ���
      regval|=(0<<7)|(1<<6)|(1<<5); 
      break;
    case D2U_L2R://���µ���,������
      regval|=(1<<7)|(0<<6)|(1<<5); 
      break;
    case D2U_R2L://���µ���,���ҵ���
      regval|=(1<<7)|(1<<6)|(1<<5); 
      break;	 
    }
  dirreg=0X3600;	   
  LCD_WriteReg(dirreg,regval);
  if(regval&0X20)
  {
    if(tftlcd_data.width<tftlcd_data.height)//����X,Y
    {
      temp=tftlcd_data.width;
      tftlcd_data.width=tftlcd_data.height;
      tftlcd_data.height=temp;
    }
  }
  else  
  {
    if(tftlcd_data.width>tftlcd_data.height)//����X,Y
    {
      temp=tftlcd_data.width;
      tftlcd_data.width=tftlcd_data.height;
      tftlcd_data.height=temp;
    }
  }  
  LCD_WR_REG(tftlcd_data.setxcmd);LCD_WR_DATA(0); 
  LCD_WR_REG(tftlcd_data.setxcmd+1);LCD_WR_DATA(0); 
  LCD_WR_REG(tftlcd_data.setxcmd+2);LCD_WR_DATA((tftlcd_data.width-1)>>8); 
  LCD_WR_REG(tftlcd_data.setxcmd+3);LCD_WR_DATA((tftlcd_data.width-1)&0XFF); 
  LCD_WR_REG(tftlcd_data.setycmd);LCD_WR_DATA(0); 
  LCD_WR_REG(tftlcd_data.setycmd+1);LCD_WR_DATA(0); 
  LCD_WR_REG(tftlcd_data.setycmd+2);LCD_WR_DATA((tftlcd_data.height-1)>>8); 
  LCD_WR_REG(tftlcd_data.setycmd+3);LCD_WR_DATA((tftlcd_data.height-1)&0XFF);

}     

//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 

		LCD_WR_REG(tftlcd_data.setxcmd);LCD_WR_DATA(Xpos>>8); 		
		LCD_WR_REG(tftlcd_data.setxcmd+1);LCD_WR_DATA(Xpos&0XFF);			 
		LCD_WR_REG(tftlcd_data.setycmd);LCD_WR_DATA(Ypos>>8);  		
		LCD_WR_REG(tftlcd_data.setycmd+1);LCD_WR_DATA(Ypos&0XFF);			

} 		 
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
	while(i--);
}
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=tftlcd_data.width||y>=tftlcd_data.height)return 0;	//�����˷�Χ,ֱ�ӷ���		     
	LCD_SetCursor(x, y);
  LCD_WriteCmd(0X2E00);
	r=LCD_ReadData();								//dummy Read    
	r=LCD_ReadData();								//ʵ��������ɫ	
	b=LCD_ReadData(); 
	g=r&0XFF;		//����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
	g<<=8;
	
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
}	

