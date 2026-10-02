#ifndef UART_CALLBACK_H
#define UART_CALLBACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef UART_Callback_StartReceive(void);

#ifdef __cplusplus
}
#endif

#endif /* UART_CALLBACK_H */
