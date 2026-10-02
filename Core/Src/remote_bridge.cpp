#include "remote.h"
#include "remote_bridge.h"

// 遥控器使用 USART3
Remote remote(&huart3);

extern "C" void Remote_Init(void)
{
    remote.init();
}

extern "C" void Remote_OnRxEvent(
    UART_HandleTypeDef *huart, uint16_t size)
{
    remote.rxMsgCheck(huart, size);
}

extern "C" bool Remote_IsConnected(void)
{
    return remote.connect_.check();
}
