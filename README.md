## Arduino-XC - easily portable Arduino code base

Main goal of this project is to bring to your favourite microcontroller that ease of setting up, 
coding and building electronics gadgets which is a hallmark of [Arduino](https://www.arduino.org/) project.

It essentialy makes available for any microcontroller:

* the ease of:
	- setting up an electronics board in the code
	- writing code for it and getting it to work quickly
	- so you can spend your time focusing on what is crucial for your project
	- in order to quickly get that proof-of-concept working
	- (those are some weird bullet points, eh?)
* a lot of Arduino libraries for various chips, sensors, boards etc. 

Once you are over that first bump/hill of implementing and setting up the hardware abstraction layer for your favourite microcontroller,
Arduino-XC will have you writing Arduino-compatible code in no time and using well-written Arduino libraries in your projects with ease.

NOTE:
> This is NOT an Arduino IDE, Arduino IDE plugin nor it has anything to do with [Arduino IDE](https://www.arduino.cc/).


## Arduino-XC - the concept

Arduino is originally written for Atmel microcontrollers and there are more than few places in it's code-base where
Atmel-specific code is used. However, Arduino core is essentially a bunch of C++ classes which, it turns out, can be compiled
for any microcontroller if their compiler/development environment supports C++.

The only thing that needs to be moved out of the way are those Atmel specific pieces of code. The way to do that is to use
HAL. Not [the computer from 2001 Odissey](https://en.wikipedia.org/wiki/HAL_9000), but an ancient design pattern: Hardware Abstraction Layer.

Arduino-XC does exactly that.

Hours and hours have been invested into:

* replacing Atmel specific pieces of code with HAL API functions
* restructuring some parts of the Arduino code base so it becomes a bit more generic and easier to port to a different microcontroller (i.e. pin handling)
* re-implementing some other parts of the code base to make it more portable-friendly (i.e. parts of Serial and Wire classes )


## How to use it with your controller

Check the [Howto](HOWTO.md) doc


## Implemented core Arduino functions

The list is based off of [Arduino IDE reference](https://www.arduino.cc/en/Reference/HomePage) page.

If a structure/variable/function is not listed here, that means it is implemented because it is a part of the C++ language or the standard C library.


**Constants**

- [x] HIGH, LOW
- [x] INPUT, OUTPUT, INPUT_PULLUP
- [x] LED_BUILTIN

**Data Types**

- [x] String object

**Utilities**

- [x] PROGMEM

**Digital I/O**

- [x] pinMode()
- [x] digitalWrite()
- [x] digitalRead() 

**Analog I/O**

- [ ] analogReference()
- [x] analogRead()
- [x] analogWrite() - PWM 

**Due & Zero only**

- [x] analogReadResolution()
- [x] analogWriteResolution() 

**Advanced I/O**

- [ ] tone()
- [ ] noTone()
- [x] shiftOut()
- [x] shiftIn()
- [x] pulseIn() 

**Time**

- [x] millis()
- [x] micros()
- [x] delay()
- [x] delayMicroseconds() 

**Math**

- [x] min()
- [x] max()
- [x] abs()
- [x] constrain()
- [x] map()

**Characters**

- [x] isAlphaNumeric()
- [x] isAlpha()
- [x] isAscii()
- [x] isWhitespace()
- [x] isControl()
- [x] isDigit()
- [x] isGraph()
- [x] isLowerCase()
- [x] isPrintable()
- [x] isPunct()
- [x] isSpace()
- [x] isUpperCase()
- [x] isHexadecimalDigit() 

**Random Numbers**

- [x] randomSeed()
- [x] random() 

**Bits and Bytes**

- [x] lowByte()
- [x] highByte()
- [x] bitRead()
- [x] bitWrite()
- [x] bitSet()
- [x] bitClear()
- [x] bit() 

**External Interrupts**

- [x] attachInterrupt()
- [x] detachInterrupt() 

**Interrupts**

- [x] interrupts()
- [x] noInterrupts() 

**Communication**

- [x] Serial
- [x] Stream 

**USB (32u4 based boards and Due/Zero only)**

- [ ] Keyboard
- [ ] Mouse 


### Standard Libraries

- [ ] EEPROM - reading and writing to "permanent" storage
- [x] SPI - for communicating with devices using the Serial Peripheral Interface (SPI) Bus
- [x] Wire - Two Wire Interface (TWI/I2C) for sending and receiving data over a net of devices or sensors. 
- [ ] SoftwareSerial - for serial communication on any digital pins. Version 1.0 and later of Arduino incorporate Mikal Hart's NewSoftSerial library as SoftwareSerial.

- [ ] Ethernet - for connecting to the internet using the Arduino Ethernet Shield
- [ ] Firmata - for communicating with applications on the computer using a standard serial protocol.
- [ ] GSM - for connecting to a GSM/GRPS network with the GSM shield.
- [ ] LiquidCrystal - for controlling liquid crystal displays (LCDs)
- [ ] SD - for reading and writing SD cards
- [ ] Servo - for controlling servo motors
- [ ] Stepper - for controlling stepper motors
- [ ] TFT - for drawing text , images, and shapes on the Arduino TFT screen
- [ ] WiFi - for connecting to the internet using the Arduino WiFi shield
