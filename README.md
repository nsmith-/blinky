Blinky - simple interface between Sparkfun pro micro and WS2812 LEDs

LED datasheet: http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Components/LED/WS2812.pdf

To use avr tools on linux, do something like `sudo apt-get install gcc-avr binutils-avr gdb-avr avr-libc avrdude` or equivalent for non-debian systems.
To use this on mac, get `brew` and do:
```
brew tap larsimmisch/avr
brew install avr-libc
brew install avrdude --with-usb
```

Once things are installed, `make` to compile, and `make install` to upload to device.  Right after executing, short `RST` and `GND` twice quickly to enter bootloader mode.
