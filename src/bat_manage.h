#ifndef BAT_MANAGE_H
#define BAT_MANAGE_H

#include <zephyr/kernel.h>
#include "io.h"
#include "i2c.h"
#include <zephyr/logging/log.h>
#include "ext_io.h"
#include "pinmap.h"
#include "nrfx_power.h"

#define BAT_MANAGE_IC_ADDR 0x6B
#define BAT_INFO_ADDR 0x48
#define BAT_REG1D_CHARGER_STATUS_2 0x1D
#define BAT_IBAT_ADC 0x33
#define BAT_ADC_DISABLE_0 0x2F
#define BAT_ADC_DISABLE_1 0x30
#define BAT_REG1C_CHARGER_STATUS_1 0x1C
#define BAT_REG2E_ADC_CONTROL 0x2E
#define BAT_IC_TEMP 0x41
#define BAT_TEMP_ADC 0x3F
#define BAT_REG03_CURRENT_LIMIT 0x03
#define BAT_REG0F_CHARGER_CONTROL_0 0x0F
#define BAT_INPUT_CURRENT_LIMIT 0x06
#define BAT_REG10_CHARGER_CONTROL_1 0x10
#define BAT_REG3B_VBAT_ADC 0x3B



int bat_manage_check(void);
int is_bat_online(void);
int bat_manage_adc_init(void);
int bat_adc_status_get(void);
int bat_current_get(void);
int bat_charger_status_get(void);
int bat_temperature_get(void);
int bat_manage_ic_temp_get(void);
int bat_disable_ico(void);
int bat_charger_current_limit_set(uint16_t limit);
int bat_charger_control_set(void);
int bat_charger_enable_set(uint8_t enable);
int bat_reg10_charger_control_1_get(void);
int bat_reg10_disable_watchdog(void);
int bat_input_current_limit_set(uint16_t limit);
int bat_vbat_adc_get(void);

#endif // BAT_MANAGE_H