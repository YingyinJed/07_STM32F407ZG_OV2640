#ifndef __BOARD_H
#define __BOARD_H
#include "main.h"
#include "key.h"
#include "touch.h"
//-------------�궨�岿��---------
#define     TIME_100MS    100
//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
//-----io ���ݵĶ��벿�ֶ���------

//-----io ���ݵ�д�벿�ֶ���------
#define     BEEP_ON  			HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)
#define     BEEP_OF  			HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET)
#define     LED1_ON  			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define     LED1_OF  			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define     LED2_ON  			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define     LED2_OF  			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)


//--------------------------------ʱ����ؽṹ��--------------------------------
typedef struct{ //���ͷ�����и��£���Ϊ1�����º���0��		
	uint8_t tkStep;
	uint8_t slStep;
	uint8_t beepnum;
	uint32_t sys_time;	
	uint32_t slTimeCounter;
  uint32_t stTimeCounter;
  uint32_t  hal_tick_time;
  uint32_t task_times;
}STRU_SYSTEM_CTRL;

extern STRU_SYSTEM_CTRL sys;

void Delayms(unsigned int myms);
void startManage(void);
void	board_quick(void);
void	board_ticks(void);
void board_slow(void);

void TIM7_PROCESS(void);
void show_XY(struTouch *_touch);

void  Key_Process(struKEY *_key);
void touch_Process(struTouch *_touch);
void display(void);
#endif
