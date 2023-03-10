# -*- coding: utf-8 -*-
"""

"""
import time
comment = False

with open("../firmware/i2c_register_map.h") as reg_file:
    with open("MAX31865_I2C_registermap.py", "w") as pyreg:
        pyreg.write('"""\n')
        pyreg.write(' '*4 + 'Generated python register map\n')
        pyreg.write(' '*4 + 'Do NOT make any changes here!\n')
        pyreg.write(' '*4 + 'Change generator script\n')
        pyreg.write(' '*4 + 'generated at '+time.strftime('%d.%m.%y %H:%M')+'\n')
        pyreg.write('"""\n')
        pyreg.write("from enum import Enum, auto\n")
        pyreg.write("")

        for line in reg_file:
            line = line[:-1]        # strip \n
            comment_pos = line.find("//")

            if(comment_pos != -1):
                line_comment = line[comment_pos:]
                line = line[:comment_pos]
            else:
                line_comment = ''

            if(len(line) < 1):
                continue

            if '#' == line[0]:      # exclude preprocessor statements
                continue
            if '//' == line[0:1]:   # exclude lines with starting comment
                continue
            if '/*' in line:
                comment = True
            if '*/' in line:
                comment = False
                continue

            if comment:
               continue

            # here we only have lines, which we want to work with
            if '}' in line:
                continue

            if 'enum' in line:
                pyreg.write("class MAX31865_I2C_registermap(Enum):\n")
                continue

            line = line.strip()
            line = line.replace(',','')

            print(line)
            line_comment = line_comment.replace('///<','')
            line_comment = line_comment.replace('/','')

            if '=' in line:
                line, value = line.split('=')
                pyreg.write("    %-20s = %-6d #%s\n"%(line, int(value), line_comment))
            else:
                pyreg.write("    %-20s = auto() #%s\n"%(line, line_comment))


