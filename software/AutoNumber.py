# -*- coding: utf-8 -*-
"""

"""
from enum import IntEnum
import re


class AutoNumber(IntEnum):
    """
        class for creting something like enums
        from: https://stackoverflow.com/a/29265816
    """
    def __new__(cls, *args):
        numberList = re.findall(r'\d+', str(cls._member_map_))
        if len(cls.__members__) > 0 and not args:
            prevMax = max(map(int, numberList))
            value = prevMax + 1
            print(format(value, '#04x'))
        else:
            value = args[0]
            print(format(value, '#04x'))

        integer = int.__new__(cls)
        integer._value_ = value
        return integer