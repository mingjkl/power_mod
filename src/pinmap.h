#ifndef PINMAP_H
#define PINMAP_H

#include <hal/nrf_gpio.h>
#include <zephyr/kernel.h>  

// PIN MAP
// AO_NTC -> P0.01     // AO Power channel ntc analog input
// FAN -> P0.00        // FAN PWM output


// UART0_TX -> P0.09   
// UART0_RX -> P0.10

// BQ25792_BAT_CE -> P0.05     // BQ25792 Battery charger chip enable

// I2C_SCL -> P1.09
// I2C_SDA -> P0.11

// CHARGE_ENABLE -> P0.15      // Enable bat boost channel

// LO_EN2 -> P0.17     // Enable bat charger power
// LO_EN1 -> P0.18     // Enable 5V output power
// AO_EN -> P0.04      // Enable adjustable power output

// VLED_CUR_TRI1 -> P0.02      // Trigger current sample channel 1
// VLED_CUR_TRI2 -> P0.03      // Trigger current sample channel 2
// VLED_CUR_TRI3 -> P0.28    // Trigger current sample channel 3
// VLED_CUR_TRI4 -> P0.29      // Trigger current sample channel 4
// VLED_CUR_TRI5 -> P0.30    // Trigger current sample channel 5
// VLED_CUR_TRI6 -> P0.31  // Trigger current sample channel 6

// VS_IN -> P0.20         // Enable current sample circuit power

// LED -> PE -> 7          // Function LED
// USB_DET -> PE -> 6      // PD USB detection
// KEY -> PE -> 5          // Key input
// LED_B1 -> PE -> 4       // Soc LED 1
// LED_B2 -> PE -> 3       // Soc LED 2
// LED_B3 -> PE -> 2       // Soc LED 3
// BST_OD_OFF -> PE -> 0   // Bat boost output diode off
// PD_OD_OFF -> PE -> 1    // PD output diode off


// Pin definitions
#define FAN_PIN NRF_GPIO_PIN_MAP(0, 0)
#define UART0_TX_PIN NRF_GPIO_PIN_MAP(0, 9)
#define UART0_RX_PIN NRF_GPIO_PIN_MAP(0, 10)
#define AO_NTC_PIN NRF_GPIO_PIN_MAP(0, 1)
#define BQ25792_BAT_CE_PIN NRF_GPIO_PIN_MAP(0, 5)
#define I2C_SCL_PIN NRF_GPIO_PIN_MAP(1, 9)
#define I2C_SDA_PIN NRF_GPIO_PIN_MAP(0, 11)
#define CHARGE_ENABLE_PIN NRF_GPIO_PIN_MAP(0, 15)
#define LO_EN2_PIN NRF_GPIO_PIN_MAP(0, 17)
#define LO_EN1_PIN NRF_GPIO_PIN_MAP(0, 18)
#define AO_EN_PIN NRF_GPIO_PIN_MAP(0, 4)
#define VLED_CUR_TRI1_PIN NRF_GPIO_PIN_MAP(0, 2)
#define VLED_CUR_TRI2_PIN NRF_GPIO_PIN_MAP(0, 3)
#define VLED_CUR_TRI3_PIN NRF_GPIO_PIN_MAP(0, 28)
#define VLED_CUR_TRI4_PIN NRF_GPIO_PIN_MAP(0, 29)
#define VLED_CUR_TRI5_PIN NRF_GPIO_PIN_MAP(0, 30)
#define VLED_CUR_TRI6_PIN NRF_GPIO_PIN_MAP(0, 31)
#define VS_IN_PIN NRF_GPIO_PIN_MAP(0, 20)

#define BAT_CHARGER_POWER_PIN LO_EN2_PIN
#define VOUT_5V_POWER_PIN LO_EN1_PIN

#endif // PINMAP_H