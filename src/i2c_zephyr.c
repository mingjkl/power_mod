#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/i2c.h>


LOG_MODULE_REGISTER(i2c_zephyr, LOG_LEVEL_DBG);
const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));

void z_i2c_write(uint8_t ic_addr, uint8_t addr, uint8_t *data, uint8_t len)
{
	struct i2c_msg msgs[2];
	msgs[0].buf = &addr;
	msgs[0].len = 1;
	msgs[0].flags = I2C_MSG_WRITE;
	msgs[1].buf = data;
	msgs[1].len = len;
	msgs[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	i2c_transfer(i2c_dev, msgs, 2, ic_addr);
}

void z_i2c_read(uint8_t ic_addr, uint8_t addr, uint8_t *data, uint8_t len)
{
	struct i2c_msg msgs[2];
	msgs[0].buf = &addr;
	msgs[0].len = 1;
	msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;
	msgs[1].buf = data;
	msgs[1].len = len;
	msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

	i2c_transfer(i2c_dev, msgs, 2, ic_addr);
}

void z_i2c_init(void)
{
	if(!device_is_ready(i2c_dev)){
		LOG_ERR("I2C device is not ready");
		return -1;
	}
	else{
		LOG_INF("I2C device is ready");
	}
}