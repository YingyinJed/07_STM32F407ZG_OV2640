#include  "key.h"
struKEY  key;
enuKEYStatus  Get_Key_Status(GPIO_PinState nowkeyx, enuKEYStatus lastkeyxstatus,uint8_t mode)
{
    enuKEYStatus  retvalue;  
    if(mode==0)
    {//UP
      if(nowkeyx == GPIO_PIN_SET)
      {  // ����, �������ŵ�ƽ=0
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
      {  // û�а���, �������ŵ�ƽ=1
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
      {  // ����, �������ŵ�ƽ=0
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
      {  // û�а���, �������ŵ�ƽ=1
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
    nowkeyx        = KEY_UP;  // ��ȡKEY1�������ŵ�ƽ
    lastkeyxstatus = _key->key_up;   // �ϴ�KEY1��״̬
    _key->key_up = Get_Key_Status(nowkeyx, lastkeyxstatus,0);
    //////
    nowkeyx        = KEY_DOWM;  // ��ȡKEY2�������ŵ�ƽ
    lastkeyxstatus = _key->key_dowm;   // �ϴ�KEY2��״̬
    _key->key_dowm = Get_Key_Status(nowkeyx, lastkeyxstatus,1);
    //////
    nowkeyx        = KEY_LEFT;  // ��ȡKEY2�������ŵ�ƽ
    lastkeyxstatus = _key->key_left;   // �ϴ�KEY2��״̬
    _key->key_left = Get_Key_Status(nowkeyx, lastkeyxstatus,1);
    //////
    nowkeyx        = KEY_RIGHT;  // ��ȡKEY2�������ŵ�ƽ
    lastkeyxstatus = _key->key_right;   // �ϴ�KEY2��״̬
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


