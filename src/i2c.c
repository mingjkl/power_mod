#include <zephyr/kernel.h>
#include <nrfx_twi.h>
#include <hal/nrf_gpio.h>
#include <zephyr/logging/log.h>
#include "pinmap.h"

LOG_MODULE_REGISTER(iic, LOG_LEVEL_DBG);

static const nrfx_twi_t twi = NRFX_TWI_INSTANCE(0);

void twi_handler(nrfx_twi_evt_t const * p_event, void * p_context) 
{
	switch (p_event->type) {
		case NRFX_TWI_EVT_DONE:
			/* Transfer completed */
			break;
		case NRFX_TWI_EVT_ADDRESS_NACK:
			/* NACK received after sending the address */
			break;
		case NRFX_TWI_EVT_DATA_NACK:
			/* NACK received after sending a data byte */
			break;
		default:
			break;
	}
}

void i2c_init(void)
{
	int err_code;
    nrfx_twi_config_t config = NRFX_TWI_DEFAULT_CONFIG(I2C_SCL_PIN, I2C_SDA_PIN);
	config.frequency = NRF_TWI_FREQ_400K;
    err_code = nrfx_twi_init(&twi, &config, twi_handler, NULL);
	if (err_code != NRFX_SUCCESS) {
		LOG_ERR("TWI initialization failed: %d", err_code);
		return;
	}
    IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TWI_INST_GET(0)), IRQ_PRIO_LOWEST,
                       NRFX_TWI_INST_HANDLER_GET(0), 0);
    nrfx_twi_enable(&twi);
}
uint8_t buffer[2] = {0};
uint8_t i2c_write_bytes(uint8_t ic_addr, uint8_t addr, uint8_t data) 
{
	buffer[0] = addr;
	buffer[1] = data;
	nrfx_twi_xfer_desc_t xfer_desc = {
		.type = NRFX_TWI_XFER_TX,
		.address = ic_addr,
		.primary_length = sizeof(buffer),
		.p_primary_buf = buffer,
	};

	while(nrfx_twi_is_busy(&twi)){};
	nrfx_twi_xfer(&twi, &xfer_desc, 0);
	return 0;
}

uint8_t i2c_write_array(uint8_t ic_addr, uint8_t addr, uint8_t *data, uint8_t len) 
{
	uint8_t buffer[len + 1];
	buffer[0] = addr;
	for (int i = 0; i < len; i++) {
		buffer[i + 1] = data[i];
	}
	nrfx_twi_xfer_desc_t xfer_desc = {
		.type = NRFX_TWI_XFER_TX,
		.address = ic_addr,
		.primary_length = sizeof(buffer),
		.p_primary_buf = buffer,
	};

	while(nrfx_twi_is_busy(&twi)){};
	nrfx_twi_xfer(&twi, &xfer_desc, 0);
	return 0;
}


uint8_t i2c_read_bytes(uint8_t ic_addr, uint8_t addr) 
{
	uint8_t addr_buffer[1] = {addr};
	uint8_t data_buffer[1];

	nrfx_twi_xfer_desc_t xfer_desc = {
		.type = NRFX_TWI_XFER_TXRX,
		.address = ic_addr,
		.primary_length = sizeof(addr_buffer),
		.p_primary_buf = addr_buffer,
		.secondary_length = sizeof(data_buffer),
		.p_secondary_buf = data_buffer,
	};

	
	nrfx_twi_xfer(&twi, &xfer_desc, 0);
	while(nrfx_twi_is_busy(&twi)){};
	return data_buffer[0];
}

uint8_t i2c_read_array(uint8_t ic_addr, uint8_t addr, uint8_t *data, uint8_t len) 
{
	uint8_t addr_buffer[1] = {addr};
	uint8_t data_buffer[len];

	nrfx_twi_xfer_desc_t xfer_desc = {
		.type = NRFX_TWI_XFER_TXRX,
		.address = ic_addr,
		.primary_length = sizeof(addr_buffer),
		.p_primary_buf = addr_buffer,
		.secondary_length = sizeof(data_buffer),
		.p_secondary_buf = data_buffer,
	};

	
	nrfx_twi_xfer(&twi, &xfer_desc, 0);
	while(nrfx_twi_is_busy(&twi)){};
	for (int i = 0; i < len; i++) {
		data[i] = data_buffer[i];
	}
	return 0;
}


