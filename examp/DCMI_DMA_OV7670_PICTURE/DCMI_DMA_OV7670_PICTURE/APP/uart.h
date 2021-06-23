#ifndef __UART_H
#define __UART_H

#include "main.h"

#define MAX_BUFFER_SIZE 100
//--------------------------------���ڴ�����ؽṹ��--------------------------------
//���ڽ������ݻ�����
typedef struct{
		uint8_t aTxb[128];		//���ͻ�����
		uint8_t Rxb[128];
	  uint8_t cmd[128];
		uint8_t RxCount;		//UART5������
		uint8_t RxNum;    	//ȫ�����պ���ֽ�
	  uint8_t RxNew;
	  uint8_t step;
	  uint32_t timenow;		
		uint32_t timelast;
}STRU_uart;
void uart_run_start(void);
void th_uart1Manage(void);

void My_UART1_IT_Handler(void);
void USART1_DMA_IT_ON(void);

#endif
