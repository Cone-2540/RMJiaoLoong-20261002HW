#include "main.h"
#include "usart.h"
#include "uart_callback.h"
#include <string.h>

/* main.c 中定义的接收缓冲区 */
extern uint8_t rx_msg[UART_FORWARD_SIZE];

/* 中断发送期间持续有效的发送缓冲区 */
static uint8_t tx_msg[UART_FORWARD_SIZE] = {0};

/* 启动 DMA + IDLE 接收 */
HAL_StatusTypeDef UART_Callback_StartReceive(void)
{
    HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_DMA(
        &huart1,
        rx_msg,
        UART_FORWARD_SIZE
    );

    if (status == HAL_OK)
    {
        /*
         * 关闭半传输中断：
         * 本任务只处理 IDLE 和接收满缓冲区事件。
         * 每次启动 DMA 后都执行一次。
         */
        __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
    }

    return status;
}

/* IDLE 或接收满缓冲区时，由 HAL 调用 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,
                               uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        HAL_UART_RxEventTypeTypeDef event = HAL_UARTEx_GetRxEventType(huart);

        if ((event == HAL_UART_RXEVENT_IDLE) ||
            (event == HAL_UART_RXEVENT_TC))
        {
            /* 检查实际接收长度 */
            if ((Size == 0U) || (Size > UART_FORWARD_SIZE))
            {
                Error_Handler();
                return;
            }

            /* 仅复制本次收到的有效字节 */
            memcpy(tx_msg, rx_msg, Size);

            /* 按实际长度发回电脑 */
            if (HAL_UART_Transmit_IT(&huart1,
                                    tx_msg,
                                    Size) != HAL_OK)
            {
                Error_Handler();
            }
        }
    }
}

/* 本次回传完成后，启动下一次接收 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (UART_Callback_StartReceive() != HAL_OK)
        {
            Error_Handler();
        }
    }
}
