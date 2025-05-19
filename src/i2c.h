#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c_init(void);
uint8_t i2c_write_bytes(uint8_t ic_addr, uint8_t addr, uint8_t data);
uint8_t i2c_write_array(uint8_t ic_addr, uint8_t addr, uint8_t *data, uint8_t len);
uint8_t i2c_read_bytes(uint8_t ic_addr, uint8_t addr);
uint8_t i2c_read_array(uint8_t ic_addr, uint8_t addr, uint8_t *data, uint8_t len);

#endif // I2C_H