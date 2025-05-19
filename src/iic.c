#include <zephyr/kernel.h>
#include <nrfx_twi.h>
#include <hal/nrf_gpio.h>
#include <zephyr/logging/log.h>
#include "pinmap.h"

LOG_MODULE_REGISTER(iic, LOG_LEVEL_INF);

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
    nrfx_twi_config_t config = NRFX_TWI_DEFAULT_CONFIG(I2C_SCL_PIN, I2C_SDA_PIN);
    nrfx_twi_init(&twi, &config, twi_handler, NULL);
    IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TWI_INST_GET(0)), IRQ_PRIO_LOWEST,
                       NRFX_TWI_INST_HANDLER_GET(0), 0);
    nrfx_twi_enable(&twi);
}

