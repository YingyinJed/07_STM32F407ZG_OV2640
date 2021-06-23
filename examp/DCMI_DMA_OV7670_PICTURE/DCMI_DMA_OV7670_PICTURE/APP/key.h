#ifndef  __KEY_H__
#define  __KEY_H__
#include "main.h"

////////////////////////////////////
#define     KEY_RIGHT     HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin)
#define     KEY_LEFT      HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin)
#define     KEY_DOWM      HAL_GPIO_ReadPin(KEY_DOWM_GPIO_Port, KEY_DOWM_Pin)
#define     KEY_UP        HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin)
////////////////////////////////////



typedef  enum{
    KEY_NO_PRESSED     = 0,  // 键没有按下状态
    KEY_JUST_PRESSED      ,  // 键刚按下状态
    KEY_ALREADY_PRESSED   ,  // 键已经按下状态
    KEY_JUST_POPUP        ,  // 键刚弹出状态
}enuKEYStatus;

typedef  struct{
    enuKEYStatus  key_up;
    enuKEYStatus  key_dowm;
    enuKEYStatus  key_left;
    enuKEYStatus  key_right;
    uint16_t      key_up_counter;
    uint16_t      key_dowm_counter;
    uint16_t      key_left_counter;
    uint16_t      key_right_counter;
}struKEY;


////////////////////////////////////

void  KEY_GPIO_Init(void);
void  Key_Read(struKEY *);
void  KEY_Param_Init(struKEY *);


#endif
