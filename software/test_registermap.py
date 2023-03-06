# -*- coding: utf-8 -*-
"""
Created on Sat Feb 11 20:26:30 2023

@author: steinmann
"""

from MAX31865_I2C_registermap import MAX31865_I2C_registermap

print(MAX31865_I2C_registermap.I2C_STATUS_CNT.value)
print(MAX31865_I2C_registermap.I2C_MAX31865_RTD0.value)

print( MAX31865_I2C_registermap.I2C_MAX31865_RTD0.value.to_bytes(2, 'big') )