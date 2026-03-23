# Bosch Athlet Vacuum Cleaner Investigations

## How to read the content of the ATmega?

### Preconditions:

- Hardware: USBasp (USB-to-AVR programmer). Connect MISO, MOSI, SCK, RES, GND and 3.3V.
- Software: avrdude. Pitfall: The avrdude which comes with the arduino IDE is quite old and reports errors. Use the official avrdude from
https://github.com/avrdudes/avrdude/releases e.g. version 8.1.

- Use ZADIG tool to assign the libusb-win32 to the USBasp.

### Commands (avrdude)

1. Read Flash Memory

`avrdude -c usbasp -p m328pb -U flash:r:flash_dump.hex:i`

-c usbasp — specifies USBasp as programmer
-p m328pb — target is ATmega328PB
-U flash:r:flash_dump.hex:i — read flash, save as Intel HEX

2. Read EEPROM

`avrdude -c usbasp -p m328pb -U eeprom:r:eeprom_dump.hex:i`

3. Read Fuse Bits

`avrdude -c usbasp -p m328pb -U lfuse:r:lfuse.hex:h -U hfuse:r:hfuse.hex:h -U efuse:r:efuse.hex:h`

:h outputs in hexadecimal format

## How to flash new software on the ATmega?

- In arduino IDE, select a board with atmega328pb, e.g. "Atmel atmega328pb Xplained".
- Menu -> Sketch -> Export Compiled Binary
- In a windows command window:
```
C:\UwesTechnik\bosch-athlet-vacuumcleaner-investigations\arduino_sketches>C:\LegacyApp\avrdude\avrdude.exe -C C:\LegacyApp\avrdude\avrdude.conf -c usbasp -p m328pb -U flash:w:"C:\UwesTechnik\bosch-athlet-vacuumcleaner-investigations\arduino_sketches\demo_just_blink\build\atmel-avr-xminis.avr.atmega328pb_xplained_mini\demo_just_blink.ino.hex":i
Reading 928 bytes for flash from input file demo_just_blink.ino.hex
Writing 928 bytes to flash
Writing | ################################################## | 100% 0.57 s
Reading | ################################################## | 100% 0.39 s
928 bytes of flash verified

Avrdude done.  Thank you.
```


## References

* Ref1: microcontroller.net forum discussion
