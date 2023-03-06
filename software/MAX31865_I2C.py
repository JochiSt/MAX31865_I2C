# -*- coding: utf-8 -*-
"""

"""
from MAX31865_I2C_registermap import MAX31865_I2C_registermap

class MAX31865_I2C(object):
    """

    """

    def __init__(self, i2c_bus, MAX31865_I2C_ADDR = 0x17, R_REF = 4300):
        """

        Parameters
        ----------
        i2c_bus : I2C_MP_USB
            I2C Bus interface. This is based - for now - on the I2C MP USB
            from https://www.fischl.de/i2c-mp-usb/
        MAX31865_I2C_ADDR : int, optional
            address of the MAX31865 I2C adapter.
            The default is 0x17.
        R_REF : float, optional
            value of the reference resistor. Typically Ohm 4300 for PT1000 and
            430 Ohm for PT100.
            The default is 4300.

        Returns
        -------
        None.

        """
        if MAX31865_I2C_ADDR > 0x17 or MAX31865_I2C_ADDR < 0x10:
            raise ValueError("address for MAX31865 invalid")

        self.bus = i2c_bus
        self.addr = MAX31865_I2C_ADDR
        self.R_REF = R_REF

    def probe(self):
        """
        Test the connection to the MAX31865 I2C adapter. (basically a single
        address I2C scan)

        Returns
        -------
        bool
            True if the device can be accessed.

        """
        return self.bus.probe_device(self.addr)

    def readRTD(self):
        """
        readout the reading from the MAX31865

        Returns
        -------
        int
            raw ADC value

        """
        reg_addr = MAX31865_I2C_registermap.I2C_MAX31865_RTD0.value.to_bytes(2, 'big')
        bus.write_i2c_block_raw(self.addr, reg_addr)
        data = bus.read_i2c_block_raw(self.addr, 2)
        return data[1] << 7 | data[0]

    def readCounter(self):
        """
        read the status counter

        Returns
        -------
        int
            counter value, this is increased inside the firmware once every
            second

        """
        reg_addr = MAX31865_I2C_registermap.I2C_STATUS_CNT.value.to_bytes(2, 'big')
        bus.write_i2c_block_raw(self.addr, reg_addr)
        data = bus.read_i2c_block_raw(self.addr, 1)
        return int.from_bytes(data, 'big')

    def convertADC2RTD(self, ADC):
        """
        convert the raw ADC counts into the resistance connected

        Parameters
        ----------
        ADC : int
            raw ADC value in LSB.

        Returns
        -------
        RRTD : float
            calculated resistance in Ohm.

        """
        RRTD = (ADC * self.R_REF) / 2**15
        return RRTD

###############################################################################
# Test Program
from i2c_mp_usb import I2C_MP_USB as SMBus
bus = SMBus()
bus.set_baudrate(100)

try:

    max31865i2c = MAX31865_I2C(bus)

    if max31865i2c.probe():
        print('device ready')
        print("Status cnt", max31865i2c.readCounter())
        max_adc = max31865i2c.readRTD()
        print("ADC counts", max_adc)
        print("resistor  ", max31865i2c.convertADC2RTD(max_adc), "Ohm")
    else:
        print('device not ready')
except Exception as e:
    print(e)
finally:
    bus.close()