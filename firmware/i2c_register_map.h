#ifndef I2C_REGISTER_MAP_H_INCLUDED
#define I2C_REGISTER_MAP_H_INCLUDED

/**
 * \defgroup I2Cmap I2C byte address map
 * Accessing the elements of this array should be done via the defines,
 * this makes it invariant for remapping the adress space and changing
 * registers
 */
/**@{*/
enum I2C_regs{
/**
 * \defgroup RDonly readonly address map
 */
/**@{*/
    I2C_STATUS_CNT = 0,         ///< status counter
    I2C_MAX31865_RTD0,          ///< LSBs of RTD value
    I2C_MAX31865_RTD1,          ///< MSBs of RTD values
    I2C_MAX31865_FAULT,         ///< content of fault register

/**@}*/ // close RDonly

/**
 * \defgroup VERSIONINFO date and time of compilation
 */
/**@{*/
    CPP_DATE_0,         ///< M
    CPP_DATE_1,         ///< M
    CPP_DATE_2,         ///< M
    CPP_DATE_3,         ///< d
    CPP_DATE_4,         ///< d
    CPP_DATE_5,         ///< Y
    CPP_DATE_6,         ///< Y
    CPP_DATE_7,         ///< Y
    CPP_DATE_8,         ///< Y

    CPP_TIME_0,         ///< H
    CPP_TIME_1,         ///< H
    CPP_TIME_2,         ///< M
    CPP_TIME_3,         ///< M
    CPP_TIME_4,         ///< S
    CPP_TIME_5,         ///< S
/**@}*/ // close VERSIONINFO

    I2C_SIZE_READONLY,  ///< define readonly sector

    I2C_MAX31865_CONFIG,        ///< configuration of MAX31865

    BOARD_ID,           ///< Board ID from EEPROM

    I2C_SIZE            ///< maximal used I2C address
};

/**@}*/ // close RW
/**@}*/ // I2Cmap

#endif
// I2C_REGISTER_MAP_H_INCLUDED
