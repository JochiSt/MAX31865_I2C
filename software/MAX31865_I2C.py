# -*- coding: utf-8 -*-
"""
Created on Mon Feb  6 14:54:02 2023

@author: steinmann
"""
import sys

from i2c_mp_usb import I2C_MP_USB as SMBus
bus = SMBus()

MAX31865_I2C_ADDR = 0x17

bus.set_baudrate(100)

try:

    if bus.probe_device(MAX31865_I2C_ADDR):
        print('device ready')

        #bus.write_i2c_block_raw(MAX31865_I2C_ADDR, [0x00, 20, 1])
        bus.write_i2c_block_raw(MAX31865_I2C_ADDR, [0x00, 0x00])

        data = bus.read_i2c_block_raw(MAX31865_I2C_ADDR, 21)
        #bus.write_byte(MAX31865_I2C_ADDR, 0x00)

        # data = bus.read_byte(MAX31865_I2C_ADDR)
        print( data )

    else:
        print('device not ready')
except Exception as e:
    print(e)
finally:
    bus.close()