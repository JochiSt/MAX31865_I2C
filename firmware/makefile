DEVICE = atmega88
MCU    = atmega88
F_CPU  = 16000000
TARGET = MAX31865_I2C

CC      = avr-gcc
OBJCOPY = avr-objcopy

INCLUDES = -I./ -I/usr/lib/avr/include

CFLAGS   += -std=c99
CFLAGS   += -Wall -Wextra
CFLAGS   += -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS   += -Os
CFLAGS   += $(INCLUDES)

LDFLAGS  = -Wl,-gc-sections -Wl,-relax

OBJECT_FILES = main.o I2CSlave.o MAX31865_AVR/max31865.o usart.o

.PHONY: Release program fuses all size clean doc

Release: all

all: $(TARGET).hex

clean:
	rm -rf *.o *.hex *.obj

doc:
	doxygen

###############################################################################
program: $(TARGET).hex
	avrdude -c avrispmkII -p $(DEVICE) -U flash:w:"$(TARGET).hex":a

# fuses from https://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega88a&LOW=DE&HIGH=D7&EXTENDED=FD&LOCKBIT=FF
fuses:
	avrdude -c avrispmkII -p $(DEVICE) -U lfuse:w:0xDE:m -U hfuse:w:0xD7:m -U efuse:w:0xFD:m -U lock:w:0xFF:m

###############################################################################
$(TARGET).hex: $(TARGET).obj
	avr-size -C --mcu=$(MCU) $<
	$(OBJCOPY) -R .eeprom -O ihex $< $@

$(TARGET).obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@


%.hex: %.obj
	avr-size -C --mcu=$(MCU) $<
	$(OBJCOPY) -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@
