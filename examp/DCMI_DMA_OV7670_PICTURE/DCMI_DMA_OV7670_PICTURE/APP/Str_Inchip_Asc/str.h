#ifndef __STR_H
#define __STR_H
#include "main.h"

typedef struct{ //���ͷ�����и��£���Ϊ1�����º���0��		
	char buff[32];
  char buff1[32];
}BUFFER_STRRING;


void str_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
void str_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);

#endif
