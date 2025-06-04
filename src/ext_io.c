#include <zephyr/kernel.h>
#include <nrfx_twi.h>
#include <hal/nrf_gpio.h>
#include <zephyr/logging/log.h>
#include "i2c.h"
#include "zephyr/logging/log.h"
#include "ext_io.h"
#include "pinmap.h"


#define EXT_IC_ADDR               0x43  // 7-bit I2C 地址（ADDR 引脚接 GND）

#define REG_DEVICE_ID_CTRL        0x01  // 设备 ID 与控制
#define REG_IO_DIRECTION          0x03  // 引脚方向设置
#define REG_OUTPUT_STATE          0x05  // 输出状态控制
#define REG_PULL_ENABLE           0x0B  // 上拉使能
#define REG_OUTPUT_HIGH_Z         0x07  // 高阻态设置（可选）
#define REG_PULL_DOWN_UP         0x0D  // 上拉/下拉设置（可选）
#define REG_INPUT_STATUS         0x0F  // 输入状态读取

uint8_t ext_io_output_global_val = 0; 


LOG_MODULE_REGISTER(ext_io, LOG_LEVEL_DBG);

void ext_io_sw_reset(void)
{
    uint8_t val = 0x01;
    i2c_write_bytes(EXT_IC_ADDR, REG_DEVICE_ID_CTRL, val);
    k_msleep(10); // 等待 10ms
    val = 0x00;
    i2c_write_bytes(EXT_IC_ADDR, REG_DEVICE_ID_CTRL, val);
    k_msleep(10); // 等待 10ms
    LOG_DBG("External IO reset");
}

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

void ext_io_set_io_set_all_output(void)
{
    uint8_t val = 0xFF; // 全部设置为输出
    i2c_write_bytes(EXT_IC_ADDR, REG_IO_DIRECTION, val);
    LOG_DBG("Set all IO to output");
}

void ext_io_set_io_set_all_input(void)
{
    uint8_t val = 0x00; // 全部设置为输入
    i2c_write_bytes(EXT_IC_ADDR, REG_IO_DIRECTION, val);
    LOG_DBG("Set all IO to input");
}

void ext_io_set_io_direction(uint8_t io_num, enum ext_io_dir_e dir)
{
    uint8_t val = ext_io_get_io_direction();
    uint8_t new_val = 0;
    
    if(io_num > 7)
    {
        LOG_ERR("Invalid IO number: %d", io_num);
        return;
    }

    if(dir == EXT_IO_DIR_INPUT) // 设置为输入
    {
        val &= ~(1 << io_num);
    }
    else // 设置为输出
    {
        val |= (1 << io_num);
    }
    i2c_write_bytes(EXT_IC_ADDR, REG_IO_DIRECTION, val);
    new_val = ext_io_get_io_direction();

    if(new_val != val)
    {
        LOG_ERR("Failed to set IO direction: %d", io_num);
    }
    else
    {
        LOG_DBG("Set IO %d direction to %s", io_num, dir ? "output" : "input");
    }
}

uint8_t ext_io_get_output_state(void)
{

    return ext_io_output_global_val;

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
    // LOG_DBG("Output state: %s", state);
    return val;
}

void ext_io_set_output_state(uint8_t io_num, uint8_t state)
{
    uint8_t val = ext_io_get_output_state();
    uint8_t new_val = 0;
    
    if(io_num > 7)
    {
        LOG_ERR("Invalid IO number: %d", io_num);
        return;
    }
    
    if(state) // 设置为高电平
    {
        val |= (1 << io_num);
    }
    else // 设置为低电平
    {
        val &= ~(1 << io_num);
    }
    ext_io_output_global_val = val;
    i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_STATE, ext_io_output_global_val);
    new_val = ext_io_get_output_state();

    if(new_val != val)
    {
        LOG_ERR("Failed to set output state: %d", io_num);
    }
    else
    {
        LOG_DBG("Set output %d state to %s", io_num, state ? "high" : "low");
    }
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

void ext_io_set_output_high_z(uint8_t io_num, uint8_t state)
{
    uint8_t val = ext_io_get_output_high_z();
    uint8_t new_val = 0;
    
    if(io_num > 7)
    {
        LOG_ERR("Invalid IO number: %d", io_num);
        return;
    }

    if(state) // 设置为高阻态
    {
        val |= (1 << io_num);
    }
    else // 设置为推挽输出
    {
        val &= ~(1 << io_num);
    }
    i2c_write_bytes(EXT_IC_ADDR, REG_OUTPUT_HIGH_Z, val);
    new_val = ext_io_get_output_high_z();

    if(new_val != val)
    {
        LOG_ERR("Failed to set output high Z: %d", io_num);
    }
    else
    {
        LOG_DBG("Set output %d high Z to %s", io_num, state ? "high Z" : "push-pull");
    }
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
    uint8_t val = i2c_read_bytes(EXT_IC_ADDR, REG_PULL_DOWN_UP);
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

uint8_t ext_io_get_channel_state(uint8_t io_num)
{
    uint8_t val = ext_io_get_input_state();
    
    if(io_num > 7)
    {
        LOG_ERR("Invalid IO number: %d", io_num);
        return 0;
    }

    if(val & (1 << io_num))
    {
        return 1; // 高电平
    }
    else
    {
        return 0; // 低电平
    }
}

uint8_t ext_io_set_pull_enable(uint8_t io_num, uint8_t enable)
{
    uint8_t val = ext_io_get_pull_enable();
    uint8_t new_val = 0;
    
    if(io_num > 7)
    {
        LOG_ERR("Invalid IO number: %d", io_num);
        return 0;
    }

    if(enable) // 设置为使能
    {
        val |= (1 << io_num);
    }
    else // 设置为不使能
    {
        val &= ~(1 << io_num);
    }
    i2c_write_bytes(EXT_IC_ADDR, REG_PULL_ENABLE, val);
    new_val = ext_io_get_pull_enable();

    if(new_val != val)
    {
        LOG_ERR("Failed to set pull enable: %d", io_num);
        return 0;
    }
    else
    {
        LOG_DBG("Set pull enable %d to %s", io_num, enable ? "enable" : "disable");
        return 1;
    }
}

uint8_t ext_io_set_pull_Down_up(uint8_t io_num, uint8_t pull)
{
    uint8_t val = ext_io_get_pull_Down_up();
    uint8_t new_val = 0;
    
    if(io_num > 7)
    {
        LOG_ERR("Invalid IO number: %d", io_num);
        return 0;
    }

    if(pull) // 设置为上拉
    {
        val |= (1 << io_num);
    }
    else // 设置为下拉
    {
        val &= ~(1 << io_num);
    }
    i2c_write_bytes(EXT_IC_ADDR, REG_PULL_DOWN_UP, val);
    new_val = ext_io_get_pull_Down_up();

    if(new_val != val)
    {
        LOG_ERR("Failed to set pull up/down: %d", io_num);
        return 0;
    }
    else
    {
        LOG_DBG("Set pull up/down %d to %s", io_num, pull ? "pull up" : "pull down");
        return 1;
    }
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

void statu_led_set(uint8_t status)
{
    if(!status)
    {
        ext_io_set_output_state(LED_EXT_PIN, 1); // 设置为高电平
    }
    else
    {
        ext_io_set_output_state(LED_EXT_PIN, 0); // 设置为低电平
    }
}

uint8_t is_key_on(void)
{
    uint8_t key_state = !ext_io_get_channel_state(KEY_EXT_PIN);
    if(key_state == 1)
    {
        LOG_DBG("Key is pressed");
        return 1;
    }
    else
    {
        LOG_DBG("Key is not pressed");
        return 0;
    }
}

uint8_t is_usb_on(void)
{
    uint8_t usb_state = ext_io_get_channel_state(USB_EXT_PIN);
    if(usb_state == 1)
    {
        LOG_DBG("USB is connected");
        return 1;
    }
    else
    {
        LOG_DBG("USB is not connected");
        return 0;
    }
}


uint8_t ext_io_init(void)
{
    ext_io_sw_reset();

    uint8_t dev_id = i2c_read_bytes(EXT_IC_ADDR, REG_DEVICE_ID_CTRL);

    if (((dev_id >> 5) & 0x07) != 0x05) {
        LOG_ERR("Device not recognized. MF_ID = 0x%X", (dev_id >> 5) & 0x07);
        return false;
    }
    else {
        LOG_DBG("Device recognized. MF_ID = 0x%X", (dev_id >> 5) & 0x07);
    }


    ext_io_set_io_direction(LED_EXT_PIN, EXT_IO_DIR_OUTPUT);
    ext_io_set_io_direction(USB_EXT_PIN, EXT_IO_DIR_INPUT);
    ext_io_set_io_direction(KEY_EXT_PIN, EXT_IO_DIR_INPUT);
    ext_io_set_io_direction(LED_B1_EXT_PIN, EXT_IO_DIR_OUTPUT);
    ext_io_set_io_direction(LED_B2_EXT_PIN, EXT_IO_DIR_OUTPUT);
    ext_io_set_io_direction(LED_B3_EXT_PIN, EXT_IO_DIR_OUTPUT);
    ext_io_set_io_direction(BST_OD_OFF_EXT_PIN, EXT_IO_DIR_OUTPUT);
    ext_io_set_io_direction(PD_OD_OFF_EXT_PIN, EXT_IO_DIR_OUTPUT);

    ext_io_set_pull_enable(LED_EXT_PIN, 0);
    ext_io_set_pull_enable(LED_B1_EXT_PIN, 0);
    ext_io_set_pull_enable(LED_B2_EXT_PIN, 0);
    ext_io_set_pull_enable(LED_B3_EXT_PIN, 0);
    ext_io_set_pull_enable(USB_EXT_PIN, 1);  
    ext_io_set_pull_enable(KEY_EXT_PIN, 1);


    ext_io_set_pull_Down_up(USB_EXT_PIN, 0);    // 设置为下拉
    ext_io_set_pull_Down_up(KEY_EXT_PIN, 1);    // 设置为上拉

    ext_io_set_output_high_z(LED_EXT_PIN, 0); // 设置为推挽输出
    ext_io_set_output_high_z(LED_B1_EXT_PIN, 0); // 设置为推挽输出
    ext_io_set_output_high_z(LED_B2_EXT_PIN, 0); // 设置为推挽输出
    ext_io_set_output_high_z(LED_B3_EXT_PIN, 0); // 设置为推挽输出
    ext_io_set_output_high_z(BST_OD_OFF_EXT_PIN, 0); // 设置为推挽输出
    ext_io_set_output_high_z(PD_OD_OFF_EXT_PIN, 0); // 设置为推挽输出


    ext_io_set_output_state(LED_EXT_PIN, 1); 
    ext_io_set_output_state(LED_B1_EXT_PIN, 1); 
    ext_io_set_output_state(LED_B2_EXT_PIN, 1); 
    ext_io_set_output_state(LED_B3_EXT_PIN, 1);
    ext_io_set_output_state(BST_OD_OFF_EXT_PIN, 0); 
    ext_io_set_output_state(PD_OD_OFF_EXT_PIN, 0); 



    LOG_DBG("External IO initialized finished");

    return true;
}