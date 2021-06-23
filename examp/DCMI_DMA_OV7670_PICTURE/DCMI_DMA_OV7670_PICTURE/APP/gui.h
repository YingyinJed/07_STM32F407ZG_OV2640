#ifndef __GUI_H
#define __GUI_H
#include "main.h"
void gui_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void gui_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void gui_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void gui_Draw_Circle(u16 x0,u16 y0,u8 r);
void gui_fill_Clear(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
#endif
