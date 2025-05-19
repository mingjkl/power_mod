/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "io.h"
#include "i2c.h"
// #include "zephyr/logging/log.h"

#include "uart.h"

// LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);


char hello[50] = "reset\n";
int i = 0;
int main(void)
{
    uart_init();

    uart_send((uint8_t *)"== Start ==\n", strlen("== Start ==\n"));
    k_msleep(500);  // 给 UART 发送一点时间

    char hello[50] = "reset\n";
    uart_send((uint8_t *)hello, strlen(hello));

    uart_send((uint8_t *)"== After reset send ==\n", strlen("== After reset send ==\n"));

    k_msleep(1000);

    uart_send((uint8_t *)"== After sleep ==\n", strlen("== After sleep ==\n"));

    int i = 0;
    while (1)
    {
        i++;
        int len = snprintf(hello, sizeof(hello), "Hello %d\n", i);
        uart_send((uint8_t *)hello, len);
        k_msleep(200);
    }

    return 0;
}