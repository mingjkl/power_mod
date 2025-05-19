/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "io.h"
#include "i2c.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);


char hello[50] = "reset\n";
int i = 0;
int main(void)
{
	LOG_DBG("Hello World! %s\n", hello);
	i2c_init();
	io_init();
    while (1)
    {
		i++;
		LOG_DBG("Hello World! %d", i);
        k_msleep(200);
    }

    return 0;
}