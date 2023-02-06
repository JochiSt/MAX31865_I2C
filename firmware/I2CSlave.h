#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

// I2C module from https://github.com/alexf91/avr-i2c-eeprom-slave
// with modifications to adapt it to our needs

#include <avr/interrupt.h>
#include <stdint.h>

void I2C_init(uint8_t address);
void I2C_stop(void);

void I2C_setCallbacks(void (*wr)(uint16_t, uint8_t), uint8_t (*rd)(uint16_t));

inline void __attribute__((always_inline)) I2C_transmitByte(uint8_t data)
{
  TWDR = data;
}

typedef enum {
    ADDRESS_HIGH = 0,
    ADDRESS_LOW,
    WR_DATA,
} i2c_state_t;

extern void i2c_wr_callback(uint16_t address, uint8_t data);
extern uint8_t i2c_rd_callback(uint16_t address);

ISR(TWI_vect);

#endif
