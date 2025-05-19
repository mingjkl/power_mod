/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "nrfx_uarte.h"
#include "pinmap.h"

#define UARTE_INST_IDX          1

#define UART_TX_PIN             NRF_GPIO_PIN_MAP(0, 6)
#define UART_RX_PIN             NRF_GPIO_PIN_MAP(0, 8)

#define UART_RX_BUFFER_SIZE     256
#define UART_PRINTF_BUFFER_SIZE 256
#define UART_RX_END_SYMBOL      '\n'

static nrfx_uarte_t uarte_instance = NRFX_UARTE_INSTANCE(UARTE_INST_IDX);

// RX
static uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
static uint16_t rx_buffer_index = 0;
static uint8_t rx_data = 0;

// TX
static volatile bool tx_done_flag = true;

// UART handler
static void uart_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    switch (p_event->type) {
    case NRFX_UARTE_EVT_TX_DONE:
        tx_done_flag = true;
        break;

    case NRFX_UARTE_EVT_RX_DONE:
        nrfx_uarte_rx(&uarte_instance, &rx_data, sizeof(rx_data));
        rx_buffer[rx_buffer_index++] = rx_data;

        if (rx_data == UART_RX_END_SYMBOL) {
            if (tx_done_flag) {
                tx_done_flag = false;
                nrfx_uarte_tx(&uarte_instance, rx_buffer, rx_buffer_index);
            }
            rx_buffer_index = 0;
        }

        if (rx_buffer_index >= UART_RX_BUFFER_SIZE) {
            rx_buffer_index = 0;
        }
        break;

    case NRFX_UARTE_EVT_ERROR:
        // 可在此记录错误信息或复位 UART
        break;

    default:
        break;
    }
}

int uart_init(void)
{
    nrfx_err_t err;

    nrfx_uarte_config_t uarte_config = NRFX_UARTE_DEFAULT_CONFIG(UART0_TX_PIN, UART0_RX_PIN);
    uarte_config.p_context = &uarte_instance;
    uarte_config.baudrate = NRF_UARTE_BAUDRATE_115200;  // 可按需修改


    err = nrfx_uarte_init(&uarte_instance, &uarte_config, uart_handler);
    if (err != NRFX_SUCCESS) {
        return -1;
    }

    // 启动首次接收
    err = nrfx_uarte_rx(&uarte_instance, &rx_data, sizeof(rx_data));
    return (err == NRFX_SUCCESS) ? 0 : -1;
}

int uart_send(uint8_t *data, uint16_t len)
{
    if (len == 0 || data == NULL) {
        return -1;
    }

    while (!tx_done_flag);   // 阻塞等待上一次传输完成
    tx_done_flag = false;

    nrfx_err_t err = nrfx_uarte_tx(&uarte_instance, data, len);
    return (err == NRFX_SUCCESS) ? 0 : -1;
}
