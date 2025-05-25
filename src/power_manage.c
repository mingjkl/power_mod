#include "power_manage.h"

LOG_MODULE_REGISTER(power, LOG_LEVEL_DBG);

struct power_status_s {
    uint8_t is_power_on;
    uint8_t is_usb_online;
    uint8_t is_bat_online;
    uint8_t charge_enable;
    uint8_t is_5v_output;
    uint8_t is_ao_output;
};

struct power_status_s power_status = {
    .is_power_on = 0,
    .is_usb_online = 0,
    .is_bat_online = 0,
    .charge_enable = 0,
    .is_5v_output = 0,
    .is_ao_output = 0,
};

void power_manage(void)
{
    
    power_status.is_usb_online = is_usb_on();
    power_status.is_bat_online = is_bat_online();

    if (power_status.is_power_on)
    {
        if(power_status.is_usb_online)
        {
            bat_boost_enable(false);    // Disable battery boost
            if (power_status.is_bat_online)
            {
                power_status.charge_enable = 1; // Enable charging
            }
        }
        if(power_status.is_bat_online)
        {
            bat_boost_enable(true);    // Enable battery boost
            power_status.charge_enable = 0; // Disable charging
        }

        if (power_status.is_5v_output)
        {
            vout_5v_power_enable(true); // Enable 5V output
        }
        else
        {
            vout_5v_power_enable(false); // Disable 5V output
        }

        if (power_status.is_ao_output)
        {
            ao_en_enable(true); // Enable AO output
        }
        else
        {
            ao_en_enable(false); // Disable AO output
        }
    }
    else
    {
        ao_en_enable(false); // Disable AO output
        vout_5v_power_enable(false); // Disable 5V output
    }

    

}


#define THREAD_KEY_DET_STACK_SIZE 500
#define THREAD_KEY_DET_PRIORITY 5


void thread_key_det(void);

K_THREAD_DEFINE(thread_key_det_id, THREAD_KEY_DET_STACK_SIZE, 
				thread_key_det, NULL, NULL, NULL,
				THREAD_KEY_DET_PRIORITY, 0, 0);

void thread_key_det(void)
{
    k_msleep(1000); // wait initialization
    LOG_DBG("Thread key det start");

    uint8_t key_on_time = 0;
    uint8_t is_key_hold = 0;
    while (1)
    {
        if(is_key_on())
        {
            key_on_time++;
            if(key_on_time > 2 && !is_key_hold)
            {
                if(power_status.is_power_on)
                {
                    statu_led_set(0);
                    power_status.is_power_on = 0;
                }
                else
                {
                    statu_led_set(1);
                    power_status.is_power_on = 1;
                }

                is_key_hold = 1;

            }
            LOG_DBG("Key on time: %d", key_on_time);
        }
        else
        {
            key_on_time = 0;
            is_key_hold = 0;
        }
        k_msleep(1000);
    }
}
