#ifndef UART_CALLBACK_H
#define UART_CALLBACK_H

#include "main.h"

/* 每次接收和回传 4 字节 */
#define UART_FORWARD_SIZE 4U

#ifdef __cplusplus
extern "C" {
#endif

HAL_StatusTypeDef UART_Callback_StartReceive(void);

#ifdef __cplusplus
}
#endif

#endif /* UART_CALLBACK_H */
