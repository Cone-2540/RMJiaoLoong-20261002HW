#include "main.h"
#include "uart_callback.h"
#include "usart.h"

extern uint8_t rx_msg[4];

/* 启动一次 1 字节 DMA 接收，返回启动状态 */
HAL_StatusTypeDef UART_Callback_StartReceive(void)
{
  return HAL_UART_Receive_DMA(&huart1, rx_msg, 1U);
}

/* DMA 接收完成回调 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    switch (rx_msg[0])
    {
    case 'M':
      HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,
                        LED_BLUE_Pin,
                        GPIO_PIN_SET);
      break;

    case 'R':
      HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,
                        LED_BLUE_Pin,
                        GPIO_PIN_RESET);
      break;

    default:
      break;
    }

    /* Normal 模式完成后，再次启动接收 */
    if (UART_Callback_StartReceive() != HAL_OK)
    {
      Error_Handler();
    }
  }
}
