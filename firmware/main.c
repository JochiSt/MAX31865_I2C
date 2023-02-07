/***
 * Adapter for accessing the MAX31865 via I2C -
 * so basically and I2C <-> SPI bridge
 *
 * I2C write command:
 *      write register, which should be read in the next read cycle
 *
 * I2C read command:
 *      read register, which has been selected
 *
 * I2C register map:
 *      see i2c_register_map.h
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdbool.h>

// own includes
#include "io_manipulation.h"
#include "config.h"
#include "usart.h"

#include "i2c_register_map.h"

#include "I2CSlave.h"
#include "MAX31865_AVR/max31865.h"

/******************************************************************************/
uint8_t i;  // some variable for counting

/******************************************************************************/
// Board ID
uint8_t EEMEM addr_BoardID = 0x99;
uint8_t BoardID;

/******************************************************************************/
// I2C
#define I2C_ADDR            0x10    ///< I2C base address
                                    ///< (last 3 bits are set via solder jumper)

volatile uint8_t data[I2C_SIZE];    ///< data, which is accessible via I2C
volatile uint8_t addr_pntr;         ///< pointer to data in array

/******************************************************************************/
// 1Hz gate timer
#define COUNTS_1HZ  125             ///< how long do we have to count to get 1Hz
volatile uint8_t cnt1Hz;            ///< counter for counting 1Hz
volatile bool timerDone;            ///< is the timer ready

// Frequency measure
volatile uint8_t timer1_overflow;   ///< count timer 1 overflow

/******************************************************************************/
// I2C receiving

volatile bool newI2Crecv;           ///< do we have received new I2C data?

/**
 * I2C receiving function ( master to slave )
 * @param addr      address, where this data should be written to
 * @param data_in   data to be written into the memory
 */
void i2c_wr_callback(uint16_t addr, uint8_t data_in)
{
    TOGGLE(LED0);
    if(addr <= I2C_SIZE_READONLY){
        // if we are in the read only part, we should not do anything
        return;
    }else{
        // make sure, that we just write into the region, where we are allowed
        // to write into
        if(addr < I2C_SIZE){
            data[addr] = data_in;
            newI2Crecv = true;
        }
    }
}
/**
 * I2C sending (slave to master)
 * @param addr      address, which data should be read
 */
uint8_t i2c_rd_callback(uint16_t addr){
    TOGGLE(LED0);
    return data[addr];
}

/******************************************************************************/
/**
 * initialize timers:
 *  - Timer 0 (8 bit) for creating a stable 1Hz readout
 */
void initTimer(void){
/* prescaler
    CS02 CS01 CS00
    0    1    0      Clock / 8
    0    1    1      Clock / 64
    1    0    0      Clock / 256
    1    0    1      Clock / 1024
*/
    // for configuration see https://eleccelerator.com/avr-timer-calculator/
    // configure Timer 0
    TCCR0A  = (1 << WGM01); // CTC mode (mode=2)
    TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);  // prescale 1024
    // 16MHz / 1024 / 125 = 125 Hz
    OCR0A = 125-1;
    cnt1Hz = COUNTS_1HZ;
    timerDone = false;

    // allow Compare Interrupt
    TIMSK0 |= (1<<OCIE0A);
}

/**
 * Timer 0 compare interrupt handler
 * called every 125 clock counts
 */
ISR (TIMER0_COMPA_vect){
    cnt1Hz--;
    if(cnt1Hz == 0){
        cnt1Hz = COUNTS_1HZ;    // reset cnt1Hz
        timerDone = true;       // true if 1Hz done
    }
}
/******************************************************************************/

void readBoardID(void){
    // read BoardID from EEPROM
    BoardID = eeprom_read_byte(&addr_BoardID);
}

/******************************************************************************/
/**
 * initialize everything - pins, timer, etc.
 */
void setup(void){
    // setup ports
    // address selection pins all with pullup
    SET_INPUT(ADDR0);
    SET(ADDR0);

    SET_INPUT(ADDR1);
    SET(ADDR1);

    SET_INPUT(ADDR2);
    SET(ADDR2);

    ////////////////////////////////////////////////////////////////////
    // initialize timer
    initTimer();

    ////////////////////////////////////////////////////////////////////
    // EEPROM
    readBoardID();

    ////////////////////////////////////////////////////////////////////
    // setup I2C
    // init data available via I2C
    for(i=0; i < I2C_SIZE; i++){
        data[i] = 0;
    }

	// set received/requested callbacks
    I2C_setCallbacks(i2c_wr_callback, i2c_rd_callback);

    // initialize I2C
    uint8_t i2cAddr = I2C_ADDR & 0xF8;  // mask out last 3 bits, because
                                        // they are set in the next line
    i2cAddr |= (IS_SET(ADDR2) << 2);
    i2cAddr |= (IS_SET(ADDR1) << 1);
    i2cAddr |= (IS_SET(ADDR0) << 0);
    I2C_init(i2cAddr);

    ////////////////////////////////////////////////////////////////////
    // include version / compile time and date in I2C registers
    // __DATE__ is 'MMM DD YYYY'
    // __TIME__ is 'hh:mm:ss'
    data[CPP_DATE_0] = __DATE__[0]; // M
    data[CPP_DATE_1] = __DATE__[1]; // M
    data[CPP_DATE_2] = __DATE__[2]; // M

    data[CPP_DATE_3] = __DATE__[4]; // D
    data[CPP_DATE_4] = __DATE__[5]; // D

    data[CPP_DATE_5] = __DATE__[7]; // Y
    data[CPP_DATE_6] = __DATE__[8]; // Y
    data[CPP_DATE_7] = __DATE__[9]; // Y
    data[CPP_DATE_8] = __DATE__[10];// Y

    data[CPP_TIME_0] = __TIME__[0]; // h
    data[CPP_TIME_1] = __TIME__[1]; // h
                                    // :
    data[CPP_TIME_2] = __TIME__[3]; // m
    data[CPP_TIME_3] = __TIME__[4]; // m
                                    // :
    data[CPP_TIME_4] = __TIME__[6]; // s
    data[CPP_TIME_5] = __TIME__[7]; // s

    data[BOARD_ID] = BoardID;

    // init the UART
    usart_init(BAUDRATE);

    // activate global interrupts
    sei();
}
/******************************************************************************/

/**
 * main loop
 */
int main(void){
    setup();    // setup everything


    // INIT MAX31865
    max_init_port();
    init_max();

    usart_write("Compiliert at "__DATE__" - "__TIME__"\n");
    usart_write("Compiliert with GCC Version "__VERSION__"\n");

    usart_write("Everything initialised\r\n");


    _delay_ms(100);
    uint8_t status_cnt = 0; ///< status counter simple free running counter

    // Main program loop
    while(1){

        // read and reset counts from timer1
        if(timerDone){
            timerDone = false;
            usart_write("Timer done ...\n");
            usart_write("%x \n", status_cnt);

            // handle counter
            data[I2C_FREQ_STATUS_CNT] = status_cnt;           // status counter

            TOGGLE(LED1);   // toggle LED as keep alive signal

            status_cnt ++;  // increase status counter

            // READOUT MAX31685
            uint16_t max31865_rtd = max_get_data('r');
            data[I2C_MAX31865_RTD0] = max31865_rtd & 0xFF;
            data[I2C_MAX31865_RTD1] = (max31865_rtd >> 7) & 0xFF;

        } // end timer done

        // if we have received new I2C data, we have to update our
        // outputs / registers
        if( newI2Crecv ){
            // reset I2C recv notifier
       		newI2Crecv = false;


            // check, whether boardID has been changed.
            if( data[BOARD_ID] != BoardID ){
                eeprom_write_byte( &addr_BoardID, data[BOARD_ID] );
                readBoardID();
                data[BOARD_ID] = BoardID;
            }

        } // end newI2Crecv
    } // end while(1)
    return -1;  // should never happen
}

