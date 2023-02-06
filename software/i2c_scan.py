# -*- coding: utf-8 -*-
from i2c_mp_usb import I2C_MP_USB as SMBus
bus = SMBus()
bus.set_baudrate(100)

print("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f")
for i in range(0, 128, 16):
    print('{0:02x}: '.format(i), end='')
    for j in range(0, 16):
        address = i + j
        if address < 0x03 or address > 0x77:
            print("   ", end='')
            continue
        if bus.probe_device(address):
            print('{0:02x} '.format(address), end='')
        else:
            print('-- ', end='')
    print('')

bus.close()