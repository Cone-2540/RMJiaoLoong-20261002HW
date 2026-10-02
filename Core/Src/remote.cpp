/**
******************************************************************************
 * @file    remote.cpp/h
 * @brief   Remote control. 遥控器
 ******************************************************************************
 * Copyright (c) 2026 Team JiaoLong-SJTU
 * All rights reserved.
 ******************************************************************************
 */

#include "remote.h"
#include <string.h>

constexpr uint16_t REMOTE_CONNECT_TIMEOUT = 500u;

// 构造函数
Remote::Remote(UART_HandleTypeDef *huart)
    : huart_(huart), connect_(REMOTE_CONNECT_TIMEOUT)
{
    rx_len_ = 0;
    reset();
}

// 打开 UART 接收
void Remote::init()
{
    HAL_UARTEx_ReceiveToIdle_DMA(
        huart_, rx_buf, RC_RX_BUF_SIZE);

    // 关闭 DMA 半传输中断
    __HAL_DMA_DISABLE_IT(huart_->hdmarx, DMA_IT_HT);
}

// 重置遥控器数据
void Remote::reset()
{
    channel_.l_row = 1024;
    channel_.l_col = 1024;
    channel_.r_row = 1024;
    channel_.r_col = 1024;
    channel_.dial_wheel = 1024;

    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
}

// 检查串口，处理完整帧
void Remote::rxMsgCheck(
    UART_HandleTypeDef *huart, uint16_t size)
{
    if (huart != huart_)
    {
        return;
    }

    rx_len_ = size;

    // 缓冲区能够容纳一帧或两帧
    if (size == RC_FRAME_LEN || size == RC_RX_BUF_SIZE)
    {
        for (uint16_t i = 0; i < size; i += RC_FRAME_LEN)
        {
            rxMsgCallback(&rx_buf[i]);
        }
    }

    // Normal 模式下，每次接收完成后重新启动
    init();
}

// 保存当前帧，解包并刷新连接状态
void Remote::rxMsgCallback(uint8_t *rx_data_)
{
    // 参数 rx_data_ 与成员同名，用 this 区分
    memcpy(this->rx_data_, rx_data_, RC_FRAME_LEN);

    handle();
    connect_.refresh();
}

// 数据解包
void Remote::handle()
{
    const uint8_t *b = rx_data_;

    // 右摇杆：横向、纵向
    channel_.r_row =
        (b[0] | (b[1] << 8)) & 0x07FF;

    channel_.r_col =
        ((b[1] >> 3) | (b[2] << 5)) & 0x07FF;

    // 左摇杆：横向、纵向
    channel_.l_row =
        ((b[2] >> 6) | (b[3] << 2) |
         (b[4] << 10)) & 0x07FF;

    channel_.l_col =
        ((b[4] >> 1) | (b[5] << 7)) & 0x07FF;

    // 左右拨杆
    switch_.l = static_cast<RCSwitchState_e>(
        (b[5] >> 6) & 0x03);

    switch_.r = static_cast<RCSwitchState_e>(
        (b[5] >> 4) & 0x03);

    // 拨轮
    channel_.dial_wheel =
        (b[16] | (b[17] << 8)) & 0x07FF;
}
