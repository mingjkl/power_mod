#include <zephyr/kernel.h>
#include "io.h"
#include "i2c.h"
#include <zephyr/logging/log.h>
#include "ext_io.h"
#include "pinmap.h"
#include "nrfx_power.h"
#include "bat_manage.h"


LOG_MODULE_REGISTER(bat, LOG_LEVEL_DBG);


int bat_manage_check(void)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_INFO_ADDR);
    if (val == 0x08)
    {
        LOG_DBG("Battery management chip is online");
        return 1;
    }
    else
    {
        LOG_ERR("Battery management chip is offline");
    }
    return 0;
}

int is_bat_online(void)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG1D_CHARGER_STATUS_2);
    if (val & 0x01)
    {
        LOG_DBG("Battery is online");
        return 1;
    }
    else
    {
        LOG_ERR("Battery is offline");
    }
    return 0;
}

int bat_manage_adc_init(void)
{
    uint8_t val = 0x00;
    i2c_write_bytes(BAT_MANAGE_IC_ADDR, 0x01, val);
    return 0;
}

int bat_adc_status_get(void)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_ADC_DISABLE_0);
    LOG_DBG("ADC status 0: %02x", val);
    val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_ADC_DISABLE_1);
    LOG_DBG("ADC status 1: %02x", val);
    val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG2E_ADC_CONTROL);
    LOG_DBG("ADC control 0x2E: %02x", val);

    i2c_write_bytes(BAT_MANAGE_IC_ADDR, BAT_REG2E_ADC_CONTROL, 0xB0);

    return 0;
}

int bat_current_get(void)
{
    uint8_t val[2] = {0};
    i2c_read_array(BAT_MANAGE_IC_ADDR, BAT_IBAT_ADC, val, 2);
    int16_t current = (val[0] << 8) | val[1];
    LOG_DBG("Battery current: %d", current);
    return current;
}

int bat_charger_status_get(void)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG1C_CHARGER_STATUS_1);
    LOG_DBG("Charger status: %02x", val);
    return val;
}

int bat_temperature_get(void)
{
    uint8_t val[2] = {0};
    i2c_read_array(BAT_MANAGE_IC_ADDR, BAT_TEMP_ADC, val, 2);
    uint16_t temperature_val = (val[0] << 8) | val[1];
    int temperature = temperature_val  * 0.0976563;
    LOG_DBG("Battery temperature: %d", temperature);
    return temperature;
}

int bat_manage_ic_temp_get(void)
{
    uint8_t val[2] = {0};
    i2c_read_array(BAT_MANAGE_IC_ADDR, BAT_IC_TEMP, val, 2);
    uint16_t temperature_val = (val[0] << 8) | val[1];
    int temperature = -40 + (temperature_val * 0.5);
    LOG_DBG("Battery management chip temperature: %d ", temperature);
    return temperature;
}

int bat_disable_ico(void)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG0F_CHARGER_CONTROL_0);
    val &= ~((1 << 4) | (1 << 1)); // Clear EN_ICO and FORCE_ICO
    i2c_write_bytes(BAT_MANAGE_IC_ADDR, BAT_REG0F_CHARGER_CONTROL_0, val);
    val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG0F_CHARGER_CONTROL_0);
    LOG_DBG("Charger control register 0x0F: %02x", val);
    return 0;
}


int bat_charger_current_limit_set(uint16_t limit)
{
    uint8_t val[2] = {0};
    if (limit > 5000)
    {
        limit = 5000;
    }

    uint16_t limit_val = limit / 10;
    val[0] = limit_val & 0xFF;
    val[1] = (limit_val >> 8) & 0xFF;

    LOG_DBG("Charger current limit: %02x %02x", val[0], val[1]);

    bat_disable_ico();

    i2c_write_array(BAT_MANAGE_IC_ADDR, BAT_REG03_CURRENT_LIMIT, val, 2);

    uint8_t val_r[2] = {0};
    i2c_read_array(BAT_MANAGE_IC_ADDR, BAT_REG03_CURRENT_LIMIT, val_r, 2);
    uint16_t current_limit = (val_r[0] | (val_r[1] << 8)) * 10;
    LOG_DBG("Charger current limit set to: %d", current_limit);
    return current_limit;

}

int bat_charger_control_set(void)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG0F_CHARGER_CONTROL_0);
    LOG_DBG("Charger control register: %02x", val);
}

int bat_charger_enable_set(uint8_t enable)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG0F_CHARGER_CONTROL_0);
    if (enable)
    {
        val |= 0x20;
    }
    else
    {
        val &= ~0x20;
    }
    i2c_write_bytes(BAT_MANAGE_IC_ADDR, BAT_REG0F_CHARGER_CONTROL_0, val);
    return 0;
}

int bat_reg10_charger_control_1_get(void)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG10_CHARGER_CONTROL_1);
    LOG_DBG("Charger control register 1: %02x", val);
    return val;
}

int bat_reg10_disable_watchdog(void)
{
    uint8_t val = i2c_read_bytes(BAT_MANAGE_IC_ADDR, BAT_REG10_CHARGER_CONTROL_1);
    val &= ~0x07;
    i2c_write_bytes(BAT_MANAGE_IC_ADDR, BAT_REG10_CHARGER_CONTROL_1, val);
    return 0;
}


int bat_input_current_limit_set(uint16_t limit)
{
    uint8_t val[2] = {0};
    if (limit > 3300)
    {
        limit = 3300;
    }

    uint16_t limit_val = (limit - 100) / 10;
    val[0] = limit_val & 0xFF;
    val[1] = (limit_val >> 8) & 0xFF;
    i2c_write_array(BAT_MANAGE_IC_ADDR, BAT_INPUT_CURRENT_LIMIT, val, 2);

    uint8_t val_r[2] = {0};
    i2c_read_array(BAT_MANAGE_IC_ADDR, BAT_INPUT_CURRENT_LIMIT, val_r, 2);
    uint16_t current_limit = (val_r[0] | (val_r[1] << 8)) * 10;
    LOG_DBG("Input current limit set to: %d", current_limit);
    return current_limit;

}

int bat_vbat_adc_get(void)
{
    uint8_t val[2] = {0};
    i2c_read_array(BAT_MANAGE_IC_ADDR, BAT_REG3B_VBAT_ADC, val, 2);
    uint16_t vbat_val = (val[0] << 8) | val[1];
    LOG_DBG("Battery voltage: %d mV", vbat_val);
    return vbat_val;
}
