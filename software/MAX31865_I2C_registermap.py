"""
    Generated python register map
    Do NOT make any changes here!
    Change generator script
    generated at 11.02.23 20:53
"""
from enum import Enum, auto
class MAX31865_I2C_registermap(Enum):
    I2C_FREQ_STATUS_CNT  = 0      # status counter
    I2C_MAX31865_RTD0    = auto() # LSBs of RTD value
    I2C_MAX31865_RTD1    = auto() # MSBs of RTD values
    I2C_MAX31865_FAULT   = auto() # content of fault register
    CPP_DATE_0           = auto() # M
    CPP_DATE_1           = auto() # M
    CPP_DATE_2           = auto() # M
    CPP_DATE_3           = auto() # d
    CPP_DATE_4           = auto() # d
    CPP_DATE_5           = auto() # Y
    CPP_DATE_6           = auto() # Y
    CPP_DATE_7           = auto() # Y
    CPP_DATE_8           = auto() # Y
    CPP_TIME_0           = auto() # H
    CPP_TIME_1           = auto() # H
    CPP_TIME_2           = auto() # M
    CPP_TIME_3           = auto() # M
    CPP_TIME_4           = auto() # S
    CPP_TIME_5           = auto() # S
    I2C_SIZE_READONLY    = auto() # define readonly sector
    I2C_MAX31865_CONFIG  = auto() # configuration of MAX31865
    BOARD_ID             = auto() # Board ID from EEPROM
    I2C_SIZE             = auto() # maximal used I2C address
