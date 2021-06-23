#ifndef __UART_H
#define __UART_H

#include "main.h"

#define MAX_BUFFER_SIZE 100
//--------------------------------串口处理相关结构体--------------------------------
//串口接收数据缓冲区
typedef struct{
		uint8_t aTxb[128];		//发送缓冲区
		uint8_t Rxb[128];
	  uint8_t cmd[128];
		uint8_t RxCount;		//UART5计数器
		uint8_t RxNum;    	//全部接收后的字节
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
