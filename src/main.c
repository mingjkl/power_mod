/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "io.h"

int main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);
	io_init();

	while(1)
	{
		io_test();
		k_msleep(1000);
	}

	return 0;
}
