#include <util/twi.h>
#include <avr/interrupt.h>

#include "I2CSlave.h"

static void (*I2C_write)(uint16_t, uint8_t);
static uint8_t (*I2C_read)(uint16_t);

static i2c_state_t state = ADDRESS_HIGH;
static uint16_t address = 0;

void I2C_setCallbacks(void (*wr)(uint16_t, uint8_t), uint8_t (*rd)(uint16_t))
{
    I2C_write = wr;
    I2C_read  = rd;
}

void I2C_init(uint8_t address)
{
    cli();
    // load address into TWI address register
    TWAR = address << 1;
    // set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
    TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
    sei();
}

void I2C_stop(void)
{
    // clear acknowledge and enable bits
    cli();
    TWCR = 0;
    TWAR = 0;
    sei();
}

ISR(TWI_vect)
{
    switch(TW_STATUS)
    {
    /* Slave receive mode */
    case TW_SR_SLA_ACK:
        TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
        state = ADDRESS_HIGH;
        break;

    case TW_SR_DATA_ACK:
        if(state == ADDRESS_HIGH)
        {
            address = (TWDR << 8);
            state = ADDRESS_LOW;
        }
        else if(state == ADDRESS_LOW)
        {
            address |= TWDR;
            state = WR_DATA;
        }
        else if(state == WR_DATA)
        {
            uint8_t data = TWDR;
            I2C_write(address, data);
            ++address;
        }
        TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
        break;

    case TW_SR_STOP:
        TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
        break;

    /* Not allowed */
    case TW_SR_DATA_NACK:
    case TW_SR_GCALL_ACK:
    case TW_SR_ARB_LOST_GCALL_ACK:
    case TW_SR_ARB_LOST_SLA_ACK:
    case TW_SR_GCALL_DATA_ACK:
    case TW_SR_GCALL_DATA_NACK:
        break;

    /* Slave transmit mode */
    case TW_ST_SLA_ACK:
    case TW_ST_DATA_ACK:
        TWDR = I2C_read(address);
        TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
        ++address;
        break;

    case TW_ST_DATA_NACK:
        TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
        break;

    /* Not allowed */
    case TW_ST_ARB_LOST_SLA_ACK:
    case TW_ST_LAST_DATA:
        break;

    default:
        TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
        break;
    }
}
