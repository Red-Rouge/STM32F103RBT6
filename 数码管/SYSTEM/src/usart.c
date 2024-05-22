#include "usart.h"

volatile uint8_t USART_RX_BUF[USART_MAX_LEN] = {0};
volatile uint32_t USART_RX_STA = 0;
volatile uint32_t USART_RX_LEN = 0;

void USART1_Config(uint32_t baud) {
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
  USART_DeInit(USART1);

  // USART1_TX   PA.9
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // PA.9
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  // USART1_RX	  PA.10
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate = baud;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStruct);

  //���ڿ����ж�
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
  //���ڽ����ж�
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  /* ���ô���1 */
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  USART_Cmd(USART1, ENABLE);
}

int fputc(int ch, FILE *file) {
  USART1->SR; // ��ֹ��λ���޷���ӡ���ַ�
  USART_SendData(USART1, ch);
  //�ȴ��������
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
  }
  return ch;
}

uint8_t USART1_SendData(const char *__format, ...) {
  va_list args;
  va_start(args, __format);

  // ��ʽ�������������
  char buffer[256]; // ���軺������СΪ256
  vsnprintf(buffer, sizeof(buffer), __format, args);

  // ��տɱ�����б�
  va_end(args);

  USART1->SR; // ��ֹ��λ���޷���ӡ���ַ�
  // ��������
  uint16_t len = strlen(buffer);
  for (uint16_t i = 0; i < len; ++i) {
    // �ȴ����ͻ���������
    while (!(USART1->SR & USART_SR_TXE)) {
    }
    // ��������
    USART1->DR = buffer[i];
  }

  return len; // ���ط��͵��ַ���
}
void BUF_Clear(void) {
  int i = 0;
  for (; i < USART_RX_STA; i++)
    USART_RX_BUF[i] = (char)0;
  USART_RX_STA = 0;
  USART_RX_LEN = 0;
}

void USART1_IRQHandler(void) {
  //��鴮��1�Ƿ񴥷��������ݻ������ǿ��ж�

  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {

    if (USART_RX_LEN < sizeof(USART_RX_BUF))
      USART_RX_BUF[USART_RX_LEN++] = USART_ReceiveData(USART1);

    //��ս������ݻ������ǿ��жϱ�־λ
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }

  //��鴮��1�Ƿ񴥷���·�����ж�
  if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET) {
    //��Ҫ�ٶ�һ�£���������ٽ��ղ��ɹ�
    USART_ReceiveData(USART1);
    USART_RX_STA = USART_RX_LEN;
    //�����·���б�־λ
    USART_ClearITPendingBit(USART1, USART_IT_IDLE);
  }
}
