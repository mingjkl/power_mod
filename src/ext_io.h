#ifndef EXT_IO_H
#define EXT_IO_H

#include <stdint.h>



enum ext_io_dir_e{
    EXT_IO_DIR_INPUT = 0,
    EXT_IO_DIR_OUTPUT = 1,
};

// ========== User-defined IO Pin Macros ==========
// Define external IO pin mapping in your project-specific pinmap.h
// Example:
//   #define LED_EXT_PIN            3
//   #define KEY_EXT_PIN            1
//   #define USB_EXT_PIN            2
//   #define LED_B1_EXT_PIN         4
//   #define LED_B2_EXT_PIN         5
//   #define LED_B3_EXT_PIN         6
//   #define BST_OD_OFF_EXT_PIN     7
//   #define PD_OD_OFF_EXT_PIN      0




// ========== Initialization and Status ==========

/**
 * @brief Initialize FXL6408 IO expander
 * @return 1 if successful, 0 if failed
 */
uint8_t ext_io_init(void);

/**
 * @brief Read and log manufacturer ID, firmware version and reset flags
 */
void ext_ic_check(void);


// ========== IO Direction Configuration ==========

/**
 * @brief Read current IO direction register
 * @return 8-bit direction register value
 */
uint8_t ext_io_get_io_direction(void);

/**
 * @brief Set all IOs as output
 */
void ext_io_set_io_set_all_output(void);

/**
 * @brief Set all IOs as input
 */
void ext_io_set_io_set_all_input(void);

/**
 * @brief Set direction for a single IO pin
 * @param io_num IO pin number (0–7)
 * @param dir Direction (input or output)
 */
void ext_io_set_io_direction(uint8_t io_num, enum ext_io_dir_e dir);


// ========== Output Control ==========

/**
 * @brief Read current output state
 * @return 8-bit output register value
 */
uint8_t ext_io_get_output_state(void);

/**
 * @brief Set output value of a single IO pin
 * @param io_num IO pin number (0–7)
 * @param state 1 = High, 0 = Low
 */
void ext_io_set_output_state(uint8_t io_num, uint8_t state);

/**
 * @brief Set output value for multiple pins using mask
 * @param mask Bit mask specifying which pins to modify
 * @param value Desired output values for masked pins
 */
void ext_io_set_output_masked(uint8_t mask, uint8_t value);


// ========== Output Mode: Push-Pull / High-Z ==========

/**
 * @brief Read High-Z state register
 * @return 8-bit value, 1 = High-Z, 0 = Push-pull
 */
uint8_t ext_io_get_output_high_z(void);

/**
 * @brief Configure High-Z or Push-Pull mode for a pin
 * @param io_num IO pin number (0–7)
 * @param state 1 = High-Z (open-drain), 0 = Push-pull
 */
void ext_io_set_output_high_z(uint8_t io_num, uint8_t state);


// ========== Pull-up / Pull-down Configuration ==========

/**
 * @brief Read pull-enable register
 * @return 8-bit value, 1 = Pull enabled, 0 = Disabled
 */
uint8_t ext_io_get_pull_enable(void);

/**
 * @brief Enable or disable internal pull resistor for a pin
 * @param io_num IO pin number (0–7)
 * @param enable 1 = Enable pull, 0 = Disable
 * @return 1 if successful, 0 if failed
 */
uint8_t ext_io_set_pull_enable(uint8_t io_num, uint8_t enable);

/**
 * @brief Read pull-up/down configuration
 * @return 8-bit value, 1 = Pull-up, 0 = Pull-down
 */
uint8_t ext_io_get_pull_Down_up(void);

/**
 * @brief Configure pull direction for a pin
 * @param io_num IO pin number (0–7)
 * @param pull 1 = Pull-up, 0 = Pull-down
 * @return 1 if successful, 0 if failed
 */
uint8_t ext_io_set_pull_Down_up(uint8_t io_num, uint8_t pull);


// ========== Input State ==========

/**
 * @brief Read all input states
 * @return 8-bit input level of all pins
 */
uint8_t ext_io_get_input_state(void);

/**
 * @brief Read input state of a single IO pin
 * @param io_num IO pin number (0–7)
 * @return 1 = High, 0 = Low
 */
uint8_t ext_io_get_channel_state(uint8_t io_num);


// ========== Application-level API ==========

/**
 * @brief Set status LED on or off
 * @param status 1 = On, 0 = Off
 */
void statu_led_set(uint8_t status);

/**
 * @brief Check if key input is active (pressed)
 * @return 1 = Pressed, 0 = Not pressed
 */
uint8_t is_key_on(void);

/**
 * @brief Check if USB is connected
 * @return 1 = Connected, 0 = Not connected
 */
uint8_t is_usb_on(void);


#endif // EXT_IO_H
