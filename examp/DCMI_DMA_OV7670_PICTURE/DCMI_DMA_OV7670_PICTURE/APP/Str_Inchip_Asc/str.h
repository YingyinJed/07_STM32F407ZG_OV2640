#ifndef __STR_H
#define __STR_H
#include "main.h"

typedef struct{ //如果头数据有更新，则为1，更新后置0；		
	char buff[32];
  char buff1[32];
}BUFFER_STRRING;


void str_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
void str_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);

#endif
