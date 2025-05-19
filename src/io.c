#include "pinmap.h"
#include <zephyr/kernel.h>
#include <hal/nrf_gpio.h>

#include <zephyr/logging/log.h>

#include "io.h"
#include "ext_io.h"

LOG_MODULE_REGISTER(io, LOG_LEVEL_INF);

void io_init(void)
{
    nrf_gpio_cfg_output(BQ25792_BAT_CE_PIN);
    nrf_gpio_cfg_output(CHARGE_ENABLE_PIN);
    nrf_gpio_cfg_output(BAT_CHARGER_POWER_PIN);
    nrf_gpio_cfg_output(VOUT_5V_POWER_PIN);
    nrf_gpio_cfg_output(AO_EN_PIN);
    nrf_gpio_cfg_output(VLED_CUR_TRI1_PIN);
    nrf_gpio_cfg_output(VLED_CUR_TRI2_PIN);
    nrf_gpio_cfg_output(VLED_CUR_TRI3_PIN);
    nrf_gpio_cfg_output(VLED_CUR_TRI4_PIN);
    nrf_gpio_cfg_output(VLED_CUR_TRI5_PIN);
    nrf_gpio_cfg_output(VLED_CUR_TRI6_PIN);
    nrf_gpio_cfg_output(VS_IN_PIN);

    nrf_gpio_pin_set(BQ25792_BAT_CE_PIN);
    nrf_gpio_pin_clear(CHARGE_ENABLE_PIN);
    nrf_gpio_pin_clear(BAT_CHARGER_POWER_PIN);
    nrf_gpio_pin_clear(VOUT_5V_POWER_PIN);
    nrf_gpio_pin_clear(AO_EN_PIN);

    nrf_gpio_pin_clear(VLED_CUR_TRI1_PIN);
    nrf_gpio_pin_clear(VLED_CUR_TRI2_PIN);
    nrf_gpio_pin_clear(VLED_CUR_TRI3_PIN);
    nrf_gpio_pin_clear(VLED_CUR_TRI4_PIN);
    nrf_gpio_pin_clear(VLED_CUR_TRI5_PIN);
    nrf_gpio_pin_clear(VLED_CUR_TRI6_PIN);

    nrf_gpio_pin_set(VS_IN_PIN);

    // ext_ic_check();

}   

void io_test(void)
{
    bat_charger_enable(1);
    bat_boost_enable(1);
    bat_charger_power_enable(1);
    vout_5v_power_enable(1);
    ao_en_enable(1);
    current_sample_power_enable(1);
    k_msleep(1000);

    bat_charger_enable(0);
    bat_boost_enable(0);
    bat_charger_power_enable(0);
    vout_5v_power_enable(0);
    ao_en_enable(0);
    current_sample_power_enable(0);
    k_msleep(1000);
}

void bat_charger_enable(uint8_t enable)
{
    if (enable) {
        nrf_gpio_pin_clear(BQ25792_BAT_CE_PIN);
    } else {
        nrf_gpio_pin_set(BQ25792_BAT_CE_PIN);
    }
}

void bat_boost_enable(uint8_t enable)
{
    if (enable) {
        nrf_gpio_pin_set(CHARGE_ENABLE_PIN);
    } else {
        nrf_gpio_pin_clear(CHARGE_ENABLE_PIN);
    }
}

void bat_charger_power_enable(uint8_t enable)
{
    if (enable) {
        nrf_gpio_pin_set(BAT_CHARGER_POWER_PIN);
    } else {
        nrf_gpio_pin_clear(BAT_CHARGER_POWER_PIN);
    }
}

void vout_5v_power_enable(uint8_t enable)
{
    if (enable) {
        nrf_gpio_pin_set(VOUT_5V_POWER_PIN);
    } else {
        nrf_gpio_pin_clear(VOUT_5V_POWER_PIN);
    }
}

void ao_en_enable(uint8_t enable)
{
    if (enable) {
        nrf_gpio_pin_set(AO_EN_PIN);
    } else {
        nrf_gpio_pin_clear(AO_EN_PIN);
    }
}

void current_sample_power_enable(uint8_t enable)
{
    if (enable) {
        nrf_gpio_pin_clear(VS_IN_PIN);
    } else {
        nrf_gpio_pin_set(VS_IN_PIN);
    }
}
