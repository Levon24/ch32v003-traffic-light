/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "debug.h"


/* Global define */
#define RED_LED_PIN     GPIO_Pin_5 
#define YELLOW_LED_PIN  GPIO_Pin_6
#define GREEN_LED_PIN   GPIO_Pin_7

/* Global Variable */


/*********************************************************************
 * @fn      USARTx_CFG
 * @brief   Initializes the USART2 & USART3 peripheral.
 * @return  none
 */
void USARTx_CFG(void) {
  GPIO_InitTypeDef  GPIO_InitStructure = {0};
  USART_InitTypeDef USART_InitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

  /* USART1 TX-->D.5 RX-->D.6 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}

/*********************************************************************
 * @brief Setup PORT C to output
 */
void PORTC_CFG(void) {
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      main
 * @brief   Main program.
 * @return  none
 */
int main(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  SystemCoreClockUpdate();
  Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
  SDI_Printf_Enable();
#else
  USART_Printf_Init(115200);
#endif
  printf("SystemClk: %d\r\n", SystemCoreClock);
  printf("ChipID: %08x\r\n", DBGMCU_GetCHIPID() );

  //USARTx_CFG();
  PORTC_CFG();

  while (1) {
    // Red
    GPIO_WriteBit(GPIOC, RED_LED_PIN, Bit_SET);
    Delay_Ms(10000);
    
    // Yellow
    GPIO_WriteBit(GPIOC, YELLOW_LED_PIN, Bit_SET);
    Delay_Ms(500);
    
    GPIO_WriteBit(GPIOC, RED_LED_PIN, Bit_RESET);
    Delay_Ms(2000);

    GPIO_WriteBit(GPIOC, YELLOW_LED_PIN, Bit_RESET);

    // Green
    GPIO_WriteBit(GPIOC, GREEN_LED_PIN, Bit_SET);
    Delay_Ms(10000);

    for (uint8_t i = 0; i < 3; i++) {
      GPIO_WriteBit(GPIOC, GREEN_LED_PIN, Bit_SET);
      Delay_Ms(500);

      GPIO_WriteBit(GPIOC, GREEN_LED_PIN, Bit_RESET);
      Delay_Ms(500);
    }

    // Yellow
    GPIO_WriteBit(GPIOC, YELLOW_LED_PIN, Bit_SET);
    Delay_Ms(1000);

    GPIO_WriteBit(GPIOC, YELLOW_LED_PIN, Bit_RESET);
  }
}
