#ifndef REMOTE_BRIDGE_H
#define REMOTE_BRIDGE_H

#include "usart.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void Remote_Init(void);

void Remote_OnRxEvent(
    UART_HandleTypeDef *huart, uint16_t size);

bool Remote_IsConnected(void);

#ifdef __cplusplus
}
#endif

#endif
