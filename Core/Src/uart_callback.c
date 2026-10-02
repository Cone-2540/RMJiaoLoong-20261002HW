#include "uart_callback.h"
#include "usart.h"

static uint8_t uart1_rx_byte;

HAL_StatusTypeDef UART_Callback_StartReceive(void)
{
  return HAL_UART_Receive_IT(&huart1, &uart1_rx_byte, 1U);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    switch (uart1_rx_byte)
    {
    case 'R':
      HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,
                        LED_BLUE_Pin,
                        GPIO_PIN_RESET);
      break;

    case 'M':
      HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,
                        LED_BLUE_Pin,
                        GPIO_PIN_SET);
      break;

    default:
      break;
    }

    /* Each completed one-byte transfer must arm the next reception. */
    if (UART_Callback_StartReceive() != HAL_OK)
    {
      Error_Handler();
    }
  }
}
