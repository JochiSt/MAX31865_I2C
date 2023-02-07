# -*- coding: utf-8 -*-
"""

"""

class MAX31865_I2C(object):
    CNT_REG_ADDR = [0x00, 0x00]
    RTD_REG_ADDR = [0x00, 0x01]

    def __init__(self, i2c_bus, MAX31865_I2C_ADDR = 0x17):
        self.bus = i2c_bus
        self.addr = MAX31865_I2C_ADDR

    def probe(self):
        return self.bus.probe_device(self.addr)

    def readRTD(self):
        bus.write_i2c_block_raw(self.addr, MAX31865_I2C.RTD_REG_ADDR)
        data = bus.read_i2c_block_raw(self.addr, 2)
        return data[1] << 7 | data[0]

    def readCounter(self):
        bus.write_i2c_block_raw(self.addr, MAX31865_I2C.CNT_REG_ADDR)
        data = bus.read_i2c_block_raw(self.addr, 1)
        return int.from_bytes(data, 'big')

from i2c_mp_usb import I2C_MP_USB as SMBus
bus = SMBus()
bus.set_baudrate(100)

try:

    max31865i2c = MAX31865_I2C(bus)

    if max31865i2c.probe():
        print('device ready')
        print(max31865i2c.readCounter())
        print(max31865i2c.readRTD())
    else:
        print('device not ready')
except Exception as e:
    print(e)
finally:
    bus.close()