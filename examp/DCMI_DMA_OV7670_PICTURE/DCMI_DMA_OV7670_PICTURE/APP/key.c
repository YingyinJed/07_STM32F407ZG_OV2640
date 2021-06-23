#include  "key.h"
struKEY  key;
enuKEYStatus  Get_Key_Status(GPIO_PinState nowkeyx, enuKEYStatus lastkeyxstatus,uint8_t mode)
{
    enuKEYStatus  retvalue;  
    if(mode==0)
    {//UP
      if(nowkeyx == GPIO_PIN_SET)
      {  // 按下, 现在引脚电平=0
        if((lastkeyxstatus == KEY_NO_PRESSED) || (lastkeyxstatus == KEY_JUST_POPUP))
        {
            retvalue = KEY_JUST_PRESSED;
        }
        else 
        {
            retvalue = KEY_ALREADY_PRESSED;
        }
      }
      else
      {  // 没有按下, 现在引脚电平=1
          if((lastkeyxstatus == KEY_NO_PRESSED) || (lastkeyxstatus == KEY_JUST_POPUP))
          {
              retvalue = KEY_NO_PRESSED;
          }
          else 
          {
              retvalue = KEY_JUST_POPUP;
          }
      }  
    }
    else//dowm,left,right
    {
      if(nowkeyx == GPIO_PIN_RESET)
      {  // 按下, 现在引脚电平=0
        if((lastkeyxstatus == KEY_NO_PRESSED) || (lastkeyxstatus == KEY_JUST_POPUP))
        {
            retvalue = KEY_JUST_PRESSED;
        }
        else 
        {
            retvalue = KEY_ALREADY_PRESSED;
        }
      }
      else
      {  // 没有按下, 现在引脚电平=1
          if((lastkeyxstatus == KEY_NO_PRESSED) || (lastkeyxstatus == KEY_JUST_POPUP))
          {
              retvalue = KEY_NO_PRESSED;
          }
          else 
          {
              retvalue = KEY_JUST_POPUP;
          }
      }  
    }
    
    return  retvalue;
}

void  Key_Read(struKEY *_key)
{
    GPIO_PinState  nowkeyx;
    enuKEYStatus   lastkeyxstatus;
    ///////
    nowkeyx        = KEY_UP;  // 读取KEY1现在引脚电平
    lastkeyxstatus = _key->key_up;   // 上次KEY1的状态
    _key->key_up = Get_Key_Status(nowkeyx, lastkeyxstatus,0);
    //////
    nowkeyx        = KEY_DOWM;  // 读取KEY2现在引脚电平
    lastkeyxstatus = _key->key_dowm;   // 上次KEY2的状态
    _key->key_dowm = Get_Key_Status(nowkeyx, lastkeyxstatus,1);
    //////
    nowkeyx        = KEY_LEFT;  // 读取KEY2现在引脚电平
    lastkeyxstatus = _key->key_left;   // 上次KEY2的状态
    _key->key_left = Get_Key_Status(nowkeyx, lastkeyxstatus,1);
    //////
    nowkeyx        = KEY_RIGHT;  // 读取KEY2现在引脚电平
    lastkeyxstatus = _key->key_right;   // 上次KEY2的状态
    _key->key_right = Get_Key_Status(nowkeyx, lastkeyxstatus,1);
}

void  KEY_Param_Init(struKEY *_key)
{
    _key->key_up = KEY_NO_PRESSED;
    _key->key_dowm = KEY_NO_PRESSED;
    _key->key_left = KEY_NO_PRESSED;
    _key->key_right = KEY_NO_PRESSED;
    
    _key->key_up_counter = 0;
    _key->key_right_counter = 0;
    _key->key_dowm_counter = 0;
    _key->key_left_counter = 0;
}


