/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "io.h"
#include "i2c.h"
#include <zephyr/logging/log.h>
#include "ext_io.h"
#include "pinmap.h"
#include "nrfx_power.h"
#include "i2c_zephyr.h"


LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static int regout0_init(void)
{
    /* 若已为 3.3 V 则跳过 */
    if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) ==
         UICR_REGOUT0_VOUT_3V3)
        return 0;

    /* 解锁 NVMC 以写 UICR */
    NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
    while (!NRF_NVMC->READY) { }

    NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 &
                         ~UICR_REGOUT0_VOUT_Msk) |
                        UICR_REGOUT0_VOUT_3V3;

    while (!NRF_NVMC->READY) { }
    NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;

    /* 强制复位，让新电压生效 */
    NVIC_SystemReset();
    return 0; /* 不会执行到这里 */
}

int i = 0;
int main(void)
{
    regout0_init();

	LOG_DBG("Begin");
	// i2c_init();
	// io_init();

    // bat_boost_enable(0);
    // current_sample_power_enable(1);

    // bat_charger_power_enable(0);
    // bat_charger_enable(0);

    // ao_en_enable(1);

    z_i2c_init();


    while (1)
    {
        statu_led_set(1);
        k_msleep(100);
		i++;
		LOG_DBG("Hello World! %d", i);
        
        statu_led_set(0);
        k_msleep(100);
    }

    return 0;
}