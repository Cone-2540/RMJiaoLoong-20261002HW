#include "main.h"
#include "usart.h"
#include "uart_callback.h"
#include <string.h>

/* main.c 中定义的接收缓冲区 */
extern uint8_t rx_msg[UART_FORWARD_SIZE];

/* 发送缓冲区：持续有效，供中断发送使用 */
static uint8_t tx_msg[UART_FORWARD_SIZE] = {0};

/* 启动一次固定长度 DMA 接收 */
HAL_StatusTypeDef UART_Callback_StartReceive(void)
{
  return HAL_UART_Receive_DMA(&huart1,
                             rx_msg,
                             UART_FORWARD_SIZE);
}

/* 收满 4 字节后调用 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    /* 将接收数据复制到独立的发送缓冲区 */
    memcpy(tx_msg, rx_msg, UART_FORWARD_SIZE);

    /* 使用中断方式将这 4 字节发回电脑 */
    if (HAL_UART_Transmit_IT(&huart1,
                            tx_msg,
                            UART_FORWARD_SIZE) != HAL_OK)
    {
      Error_Handler();
    }

    /*
     * 下一轮接收在发送完成回调中启动，
     * 本次收发按顺序进行。
     */
  }
}

/* 4 字节全部发送完成后调用 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    /* 为下一组数据启动 DMA 接收 */
    if (UART_Callback_StartReceive() != HAL_OK)
    {
      Error_Handler();
    }
  }
}
