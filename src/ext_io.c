#include <zephyr/kernel.h>
#include <nrfx_twi.h>
#include <hal/nrf_gpio.h>
#include <zephyr/logging/log.h>
#include "i2c.h"
#include "zephyr/logging/log.h"

#define EXT_IC_ADDR 0x43

LOG_MODULE_REGISTER(ext_io, LOG_LEVEL_DBG);

void ext_ic_check(void)
{
    uint8_t reg_val = i2c_read_bytes(EXT_IC_ADDR, 0x01);

    uint8_t mf_id     = (reg_val >> 5) & 0x07;  // Bit 7~5
    uint8_t fw_rev    = (reg_val >> 2) & 0x07;  // Bit 4~2
    uint8_t rst_int   = (reg_val >> 1) & 0x01;  // Bit 1
    uint8_t sw_rst    = (reg_val >> 0) & 0x01;  // Bit 0

    printk("Manufacturer ID: 0x%X", mf_id);   // 应为 0b101 = 0x5
    printk("Firmware Rev:   0x%X", fw_rev);
    printk("Reset Flag:     %d", rst_int);
    printk("SW Reset Flag:  %d", sw_rst);
}