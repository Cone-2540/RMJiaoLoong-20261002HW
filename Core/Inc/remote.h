/**
******************************************************************************
 * @file    remote.cpp/h
 * @brief   Remote control. 遥控器
 ******************************************************************************
 * Copyright (c) 2026 Team JiaoLong-SJTU
 * All rights reserved.
 ******************************************************************************
 */
#ifndef UART_REMOTE_H
#define UART_REMOTE_H

#include "connect.hpp"
#include "usart.h"

constexpr uint16_t RC_RX_BUF_SIZE = 36u;
constexpr uint16_t RC_FRAME_LEN = 18u;

class Remote {
public:
    enum class RCSwitchState_e {
        UP = 1,
        DOWN = 2,
        MID = 3
    };

private:
    UART_HandleTypeDef *huart_;

    uint8_t rx_buf[RC_RX_BUF_SIZE];
    uint8_t rx_data_[RC_FRAME_LEN];

    volatile uint16_t rx_len_;

    void startReceive();

public:
    Connect connect_;

    /* 原始通道值：中心约 1024 */
    struct {
        uint16_t l_row;
        uint16_t l_col;
        uint16_t r_row;
        uint16_t r_col;
        uint16_t dial_wheel;
    } channel_;

    struct {
        RCSwitchState_e l;
        RCSwitchState_e r;
    } switch_;

    /* 调试计数 */
    volatile uint32_t valid_frames;
    volatile uint32_t invalid_frames;

    explicit Remote(UART_HandleTypeDef *huart);
    ~Remote() = default;

    void init();
    void reset();

    void rxMsgCallback(uint8_t *data);
    void rxMsgCheck(UART_HandleTypeDef *huart, uint16_t size);
    void handle();
};

#endif
