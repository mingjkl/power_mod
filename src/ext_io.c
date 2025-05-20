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
#define REG_PULL_DOWN_UP         0x0D  // 上拉/下拉设置（可选）
#define REG_INPUT_STATUS         0x0F  // 输入状态读取

LOG_MODULE_REGISTER(ext_io, LOG_LEVEL_DBG);

uint8_t ext_io_get_io_direction(void)
{
    uint8_t val = i2c_read_bytes(EXT_IC_ADDR, REG_IO_DIRECTION);
    char dir[16] = {0};
    for(uint8_t i = 0; i < 8; i++)
    {
        if(val & (1 << i))
        {
            snprintf(dir + i, 2, "O");  
        }
        else
        {
            snprintf(dir + i, 2, "I");
        }
    }
    LOG_DBG("IO direction: %s", dir);
    return val;
}

uint8_t ext_io_get_output_state(void)
{
    uint8_t val = i2c_read_bytes(EXT_IC_ADDR, REG_OUTPUT_STATE);
    char state[16] = {0};
    for(uint8_t i = 0; i < 8; i++)
    {
        if(val & (1 << i))
        {
            snprintf(state + i, 2, "H");  

        }
        else
        {
            snprintf(state + i, 2, "L");

        }
    }   
    LOG_DBG("Output state: %s", state);
    return val;
}

uint8_t ext_io_get_output_high_z(void)
{
    uint8_t val = i2c_read_bytes(EXT_IC_ADDR, REG_OUTPUT_HIGH_Z);
    char state[16] = {0};
    for(uint8_t i = 0; i < 8; i++)
    {
        if(val & (1 << i))
        {
            snprintf(state + i, 2, "Z");  
        }
        else
        {
            snprintf(state + i, 2, "P");
        }
    }   
    LOG_DBG("Output high Z: %s", state);
    return val;
}

uint8_t ext_io_get_pull_enable(void)
{
    uint8_t val = i2c_read_bytes(EXT_IC_ADDR, REG_PULL_ENABLE);
    char state[16] = {0};
    for(uint8_t i = 0; i < 8; i++)
    {
        if(val & (1 << i))
        {
            snprintf(state + i, 2, "P");  
        }
        else
        {
            snprintf(state + i, 2, "N");
        }
    }   
    LOG_DBG("Pull enable: %s", state);
    return val;
}

uint8_t ext_io_get_pull_Down_up(void)
{
    uint8_t val = i2c_read_bytes(EXT_IC_ADDR, REG_PULL_ENABLE);
    char state[16] = {0};
    for(uint8_t i = 0; i < 8; i++)
    {
        if(val & (1 << i))
        {
            snprintf(state + i, 2, "U");  
        }
        else
        {
            snprintf(state + i, 2, "D");
        }
    }   
    LOG_DBG("Pull up/down: %s", state);
    return val;
}

uint8_t ext_io_get_input_state(void)
{
    uint8_t val = i2c_read_bytes(EXT_IC_ADDR, REG_INPUT_STATUS);
    char state[16] = {0};
    for(uint8_t i = 0; i < 8; i++)
    {
        if(val & (1 << i))
        {
            snprintf(state + i, 2, "H");  
        }
        else
        {
            snprintf(state + i, 2, "L");
        }
    }   
    LOG_DBG("Input state: %s", state);
    return val;
}


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





uint8_t ext_io_init(void)
{
    // uint8_t dev_id = i2c_read_bytes(EXT_IC_ADDR, REG_DEVICE_ID_CTRL);

    // if (((dev_id >> 5) & 0x07) != 0x05) {
    //     LOG_ERR("Device not recognized. MF_ID = 0x%X", (dev_id >> 5) & 0x07);
    //     return false;
    // }

    // // 默认初始化为全部推挽输出，输出低电平，禁用上拉
    // i2c_write_bytes(EXT_IC_ADDR, REG_IO_DIRECTION, 0xFF);   // 全为输出
    // i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_STATE,     0x00);  // 初始低电平
    // i2c_write_bytes(EXT_IC_ADDR, REG_PULL_ENABLE,      0x00);  // 禁用上拉
    // i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_HIGH_Z,    0x00);  // 关闭高阻态（全推挽）

    // ext_io_test_blinky();

    while(1)
    {
        // i2c_read_bytes(EXT_IC_ADDR, REG_DEVICE_ID_CTRL);
        ext_io_get_io_direction();
        k_msleep(10);
        ext_io_get_output_state();
        k_msleep(10);
        ext_io_get_output_high_z();
        k_msleep(10);
        ext_io_get_pull_enable();
        k_msleep(10);
        ext_io_get_pull_Down_up();
        k_msleep(10);
        ext_io_get_input_state();
        k_msleep(10);
    }

    return true;
}