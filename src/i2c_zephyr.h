#ifndef I2C_ZEPHYR_H
#define I2C_ZEPHYR_H

#include <stdint.h>
#include <zephyr/drivers/i2c.h>

void z_i2c_write(uint8_t ic_addr, uint8_t addr, uint8_t *data, uint8_t len);
void z_i2c_read(uint8_t ic_addr, uint8_t addr, uint8_t *data, uint8_t len);
void z_i2c_init(void);

#endif // I2C_ZEPHYR_H