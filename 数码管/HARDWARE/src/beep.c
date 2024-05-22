#include "beep.h"

void BEEP_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Pin = BEEP_PIN;
  GPIO_Init(BEEP_PORT, &GPIO_InitStruct);

  GPIO_SetBits(BEEP_PORT, BEEP_PIN);
}
