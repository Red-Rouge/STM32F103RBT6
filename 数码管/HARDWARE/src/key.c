#include "key.h"

uint8_t key_state;
static uint8_t state;

void KEY_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
                             RCC_APB2Periph_GPIOD,
                         ENABLE);
  GPIO_StructInit(&GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStruct);
}

uint8_t KEY_Scan(uint8_t mode) {
  GPIO_TypeDef *gpio[] = {GPIOC, GPIOC, GPIOD};
  // 定义引脚
  uint16_t pin[] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_2};

  if (Pin(GPIOA, GPIO_Pin_0) == 1) {
    delay_ms(20);
    if (Pin(GPIOA, GPIO_Pin_0) == 1) {
      while (Pin(GPIOA, GPIO_Pin_0) == 1) {
      }
      return 1;
    }
  }
  for (uint8_t i = 0; i < 3; i++) {
    if (Pin(gpio[i], pin[i]) == 0) {
      delay_ms(20);
      if (Pin(gpio[i], pin[i]) == 0) {
        while (Pin(gpio[i], pin[i]) == 0) {
        }
        return i + 2;
      }
    }
  }
  return 0;
}

void EXTI_Config(void) {
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);

  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);

  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct.EXTI_Line = EXTI_Line8 | EXTI_Line9 | EXTI_Line2;
  EXTI_Init(&EXTI_InitStruct);

  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  TIM_TimeBaseStruct.TIM_Period = 20;
  TIM_TimeBaseStruct.TIM_Prescaler = 7200 - 1;
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

void EXTI0_IRQHandler(void) {
  NVIC_DisableIRQ(EXTI0_IRQn);
  if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
    state = Pin(GPIOA, GPIO_Pin_0);
    START;
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
  NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI9_5_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
    state = Pin(GPIOC, GPIO_Pin_8);
    START;
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
  if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
    state = Pin(GPIOC, GPIO_Pin_9);
    START;
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
}

void EXTI2_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
    state = Pin(GPIOD, GPIO_Pin_2);
    START;
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}

void TIM2_IRQHandler(void) {
  GPIO_TypeDef *gpio[] = {GPIOA, GPIOC, GPIOC, GPIOD};
  uint16_t pin[] = {GPIO_Pin_0, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_2};

  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    for (int i = 0; i < 4; i++) {
      uint8_t cur_state = Pin(gpio[i], pin[i]);
      if (cur_state == state &&
          (cur_state == (pin[i] == GPIO_Pin_0) ? (Bit_SET) : (Bit_RESET))) {
        key_state = i + 1;
      }
    }
    TIM_SetCounter(TIM5, 0);
    STOP;
  }
}
