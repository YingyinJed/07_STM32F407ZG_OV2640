#include "tim.h"
FunctionalState  TIM7_EN;
extern TIM_HandleTypeDef htim7;
// 定时器溢出中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM7)
    {
        TIM7_EN = ENABLE;
    }
}

void USER_TIM7_Init(uint16_t prescaler, uint16_t period)
{
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = prescaler;
  htim7.Init.Period = period;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
}

