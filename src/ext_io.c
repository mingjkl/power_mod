#include <zephyr/kernel.h>
#include <nrfx_twi.h>
#include <hal/nrf_gpio.h>
#include <zephyr/logging/log.h>
#include "i2c.h"
#include "zephyr/logging/log.h"


#define EXT_IC_ADDR               0x43  // 7-bit I2C 地址（ADDR 引脚接 GND）

#define REG_DEVICE_ID_CTRL        0x01  // 设备 ID 与控制
#define REG_IO_DIRECTION          0x03  // 引脚方向设置
#define REG_OUTPUT_STATE          0x05  // 输出状态控制
#define REG_PULL_ENABLE           0x0B  // 上拉使能
#define REG_OUTPUT_HIGH_Z         0x07  // 高阻态设置（可选）


LOG_MODULE_REGISTER(ext_io, LOG_LEVEL_DBG);

void ext_ic_check(void)
{
    uint8_t reg_val = i2c_read_bytes(EXT_IC_ADDR, 0x01);

    uint8_t mf_id     = (reg_val >> 5) & 0x07;  // Bit 7~5
    uint8_t fw_rev    = (reg_val >> 2) & 0x07;  // Bit 4~2
    uint8_t rst_int   = (reg_val >> 1) & 0x01;  // Bit 1
    uint8_t sw_rst    = (reg_val >> 0) & 0x01;  // Bit 0

    LOG_DBG("Manufacturer ID: 0x%X", mf_id);   // 应为 0b101 = 0x5
    LOG_DBG("Firmware Rev:   0x%X", fw_rev);
    LOG_DBG("Reset Flag:     %d", rst_int);
    LOG_DBG("SW Reset Flag:  %d", sw_rst);
}

void ext_io_set_output(uint8_t pin, bool high)
{
    if (pin > 7) return;

    uint8_t val = i2c_read_bytes(EXT_IC_ADDR, REG_OUTPUT_STATE);

    LOG_DBG("Current output state: 0x%X", val);


    if (high)
        val |= (1 << pin);
    else
        val &= ~(1 << pin);

    LOG_DBG("New output state: 0x%X", val);

    i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_STATE, val);
}

void ext_io_config_pin(uint8_t pin, bool is_input, bool pullup)
{
    if (pin > 7) return;

    // --- 设置方向寄存器 ---
    uint8_t dir = i2c_read_bytes(EXT_IC_ADDR, REG_IO_DIRECTION);
    if (is_input)
        dir &= ~(1 << pin);  // 0 = 输入
    else
        dir |= (1 << pin);   // 1 = 输出
    i2c_write_bytes(EXT_IC_ADDR, REG_IO_DIRECTION, dir);

    // --- 设置上拉使能 ---
    uint8_t pull = i2c_read_bytes(EXT_IC_ADDR, REG_PULL_ENABLE);
    if (is_input && pullup)
        pull |= (1 << pin);
    else
        pull &= ~(1 << pin);
    i2c_write_bytes(EXT_IC_ADDR, REG_PULL_ENABLE, pull);

    // --- 可选项：设置输出高阻（推挽或开漏）---
    // i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_HIGH_Z, 0x00); // 默认推挽
}

void ext_io_test_blinky(void)
{
    ext_io_config_pin(2, false, false);  // 设置 P2 为输出
    ext_io_config_pin(3, false, false);  // 设置 P3 为输出
    ext_io_config_pin(4, false, false);   // 设置 P4 为输入，禁用上拉

    while (1) {
        // ext_io_set_output(2, true);
        // ext_io_set_output(3, true);
        // ext_io_set_output(4, true);
        i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_STATE, 0xFF);
        LOG_DBG("LED ON");
        k_msleep(500);

        // ext_io_set_output(2, false);
        // ext_io_set_output(3, false);
        // ext_io_set_output(4, false);
        // i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_STATE, 0x00);
        LOG_DBG("LED OFF");
        k_msleep(500);
    }
}

uint8_t ext_io_init(void)
{
    uint8_t dev_id = i2c_read_bytes(EXT_IC_ADDR, REG_DEVICE_ID_CTRL);

    if (((dev_id >> 5) & 0x07) != 0x05) {
        LOG_ERR("Device not recognized. MF_ID = 0x%X", (dev_id >> 5) & 0x07);
        return false;
    }

    // 默认初始化为全部推挽输出，输出低电平，禁用上拉
    i2c_write_bytes(EXT_IC_ADDR, REG_IO_DIRECTION, 0xFF);   // 全为输出
    i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_STATE,     0x00);  // 初始低电平
    i2c_write_bytes(EXT_IC_ADDR, REG_PULL_ENABLE,      0x00);  // 禁用上拉
    i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_HIGH_Z,    0x00);  // 关闭高阻态（全推挽）

    ext_io_test_blinky();

    return true;
}