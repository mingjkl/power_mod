#ifndef IO_H
#define IO_H

void io_init(void);
void io_test(void);
void bat_charger_enable(uint8_t enable);
void bat_boost_enable(uint8_t enable);
void bat_charger_power_enable(uint8_t enable);
void vout_5v_power_enable(uint8_t enable);
void ao_en_enable(uint8_t enable);
void current_sample_power_enable(uint8_t enable);


#endif // IO_H