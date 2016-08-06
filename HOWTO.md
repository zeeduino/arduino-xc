## Zeeduino-XC on your microcontroller



### Using the library with already implemented HAL

Check the [LPC1347 Zeeduino-XC HAL implementation](https://github.com/zeeduino/zeeduino-xc-hal-lpc13xx) for example of using already implemented
HAL in your project.

In this case it is implementation for NXP LPC1347 micro which is then used with LPCXpresso IDE to create and run Zeeduino-XC projects.



### Implementing the HAL yourself

> If you are contemplating of implementing HAL layer for your favourite microcontroller yourself, keep in mind that this is not a novice exercise, you should have sufficient knowledge and experience building projects and writing code for your controller before you start. Because hardly any coding ever goes without hiccups and this one is bound to get you to a hard spot or two until the initial HAL implementation is done and working.
> 
> After that, it is a smooth ride, having Arduino/Wiring code running on your chip.

Also keep in mind that **you will have to do all this work down below just once for your chip. Once you've done that, every time you create a new sketch, all you have to do is reference Zeeduino-XC, Zeeduino-XC-HAL-your_port and chip support libraries in your project and then go on and code your application!**.

In order for Zeeduino-XC to work on your microcontroler, you will have to provide pieces of code which are specific to your chosen chip.

Process is actually fairly simple and can be done in steps where after each step you'll have something to play with.

In order to help you start faster, there are two projects you can use:

- Zeeduino-XC HAL porting base code: https://github.com/zeeduino/zeeduino-xc-hal
- basic sketch project for LPCXpresso IDE: https://github.com/zeeduino/zeeduino-xc-basicSketch

The portbase project contains all the source files you'll need to make Zeeduino-XC work on your microcontroller.

In order to have HAL implemented for your controller, you'll have to:

- prepare the environment in your development IDE for working on HAL implementation
    * build Zeeduino-XC library
    * download and build chip specific support libraries
    * create and build initial dummy HAL implementation project
    * create basic sketch project
- implement HAL in 5 steps (or more :) )
    * setup pinout
    * implement digital functions
    * implement delay functions
    * implement serial object
    * implement other pieces when you need them

So, let's start!


#### Zeeduino-XC library

First thing to do is to create a C++ library project using your chip's development environment and [Zeeduino-XC source](https://github.com/zeeduino/zeeduino-xc-core). Check [this video][Zeeduino-XC library on lpc1347] to see how we did it for LPC1347 and LPCXpresso IDE.

Zeeduino-XC core code is microcontroller-agnostic and should compile with no issues on any standard C++ compiler.

Make sure to add a symbol definition to your project setup:

- name: **ARDUINO**
- value: **200**

C/C++ preprocessor and compiler will use this symbol to correctly include and compile various Arduino libraries.


Once Zeeduino-XC core is compiled and you have the library built, it's time to prepare for implementing HAL.


#### Chip-specific support libraries

These days, every microcontroller manufacturer has some form of basic support code available for their chips. This is usually in a form of a library like [LPCOpen] for LPC controllers or [Atmel Software Framework] for AVR and SAM controllers, etc. Or it may be a bunch of application notes and some source code and examples to get you started quickly with that chip.

Whatever is the case, you should get those libraries (or library) for your chosen chip and build and test them with simple blinky led program. We are not going to go into details of doing that since it is highly specific to whatever controller you are currently using.

Once you have your chip's support library in place and working, it's time to create Zeeduino-XC HAL implementation library for your microcontroller.


#### HAL implementation library initial setup

Clone the [portbase code](https://github.com/zeeduino/zeeduino-xc-hal) and rename it to something that will be specific to your microcontroller, i.e. zeeduino-xc-hal-lpc13xx.

Now you need to create a C++ library project using your chip's development environment and add portbase source to it. You will also have to add references to Zeeduino-XC library to the portbase project. Check out [this video][creating portbase library for lpc11u37] to see how we did it for LPC11U37 using LPCXpresso IDE. It will certainly be different for your controller, but it helps to get the general idea of what you need to do.

The most important thing is that Zeeduino-XC include (**`inc`**) folder is referenced in your portbase project.

Next important thing is that your chip support library (LPCOpen, Atmel Software Framework, etc.) is referenced by your portbase project.

Those two important things kind of underline the fact that HAL implementation library is a bridge between Zeeduino-XC core code and the chip specific code.

Having your portbase project created and set up, build it. It will probably contain errors or warnings. This is expected because zeeduino-xc-hal project contains couple of calls to functions which do not exist and are there just as a placeholder or reminder. For example, **read_the_char_from_UART()** function is not a function, it just states that you need to read a character from your chip's UART peripheral at this point.

You can comment out these places where you have errors or warnings and aim to have your portbase compile clean, with no errors and warnings.

Probably a place with the most issues will be the [pins_arduino.cpp](https://github.com/zeeduino/zeeduino-xc-hal/blob/master/src/pins_arduino.cpp) file. This file contains definitions of chip specific constants, and some may even not apply to your chip. All constants starting with **`LPC_`** and **`IOCON_`** are LPC chip specific and you can either replace them or define them as **`0`** at this point. You will have to come up with values here that are specific for your chip. More on that later.

Also, make sure to add a symbol definition to your project setup:

- name: **ARDUINO**
- value: **200**

C/C++ preprocessor and compiler will use this symbol to correctly include and compile various Arduino libraries.


Once the portbase project is set up and ready, it's time to create a basic Arduino sketch we will use to check our HAL implementation progress.


#### Basic Arduino sketch project

You can find a source code for basic sketch [here][basicSketch project]. However, that project is specific for LPC13xx controllers and the only thing you'll use from there is [basicSketch.cpp](https://github.com/zeeduino/zeeduino-xc-basicSketch/blob/master/src/basicSketch.cpp) file. We'll need other files just as an example of how to get the basic sketch working in your project.

As a quick start help, you can also use **`src/board.c`** and **`inc/board.h`** files and copy them into your project.

Create an empty C++ executable project using your chip's development environment (for short, we'll call it IDE from now on) and add basicSketch.cpp to it. Your IDE should have created a source file which contains **main()** function. If that file only contains the main() function and nothing else of importance, you should delete that file. If the file contains some important bits and pieces, like chip initialization code, you should just delete the **main()** function and leave the file in the project.

Next, add to the project references to:
- zeeduino-xc-core library
- your copy of zeeduino-xc-hal library
- your chip's support library

Order in which these are referenced in the project (not added to the project, but referenced!) might be important (for example for the linker to find all the symbols), and the one above is usually working ok.

Now we need to tie in few pieces to get this Frankenstein off the ground.


##### Chip initialization code

There usually is some sort of chip initialization code that starts up the chip, does some basic setup and then calls the **main()** function. [basicSketch project] contains `src` folder and within that folder there's a `lpcx` folder. the `lpcx` contains all the code that LPCXpressi IDE auto-generated when we created the C++ project.

You may or may not want to look at those files, their content is not relevant for our work here, except for a few pieces.

###### Call Board_SystemInit()

[sysinit.c](https://github.com/zeeduino/zeeduino-xc-basicSketch/blob/master/src/lpcx/sysinit.c) file contains `SystemInit()` function which calls **`Board_SystemInit()`**. Find a similar place in the code generated by your IDE and place the call to `Board_SystemInit()` there.

Tha main thing `Board_SystemInit()` has to do is to call **`initVariant()`** function. `Board_SystemInit()` seems like a unnecessary wrapper to call `initVariant()` but we did it this way so you can do some application, or board, specific initialization in `Board_SystemInit()`.

If you do that, however, keep in mind that your Arduino code will not be easily portable to some other microcontroller anymore. On the other hand, Zeeduino-XC is here to enable you to quickly start with a project on your microcontroller, test how it works, and then add application specific details. These application specific bits of code do not have to be portable, they only have to work and do what you need them to do in your hardware project.


###### Make sure `main()` is called from somewhere

Depending on your IDE, this may or may not be obvius: you have to make sure that you didn't remove the code that calls `main()` from your project. In our case of [basicSketch project], the `main()` is called from cr_startup_lpc13uxx.cpp file, in [`ResetISR()`](https://github.com/zeeduino/zeeduino-xc-basicSketch/blob/master/src/lpcx/cr_startup_lpc13uxx.cpp#L333).


###### Build the project

Before building your basic sketch project, you should add a symbol definition to your project setup:

- name: **ARDUINO**
- value: **200**

C/C++ preprocessor and compiler will use this symbol to correctly include and compile various Arduino libraries.

Now, build your project and work your way through any errors or warnings there may be. Goal here is to get everything to compile cleanly, at this point the code doesn't do anything useful.


#### Implementing HAL

At this point you should have:

- zeeduino-xc-core library project set up and buildable
- your version of zeeduino-xc-hal library project set up and buildable
- chip support library project set up and buildable
- basicSketch executable project set up and buildable

Now we'll:

- set up few pins on a chip as digital I/Os
- write HAL code to work with digital pins
- write HAL code to support Arduino delay functions
- run basicSketch to blink LED on our board


##### Setting up `pins_arduino.cpp`

If you take a look at [pins_arduino.h](https://github.com/zeeduino/zeeduino-xc-hal/blob/master/inc/pins_arduino.h) header file, you'll notice few type definitions of which the most important one is [`ArduinoPinDescriptionType`](https://github.com/zeeduino/zeeduino-xc-hal/blob/master/inc/pins_arduino.h#L93):

```C++
typedef struct _ArduinoPinDescriptionType
{
    uint32_t port:8;
    uint32_t pin:8;
    uint32_t modefunc:16;
    AnalogChannelType adcChannel;
    PwmChannelType pwmChannel;
    ExtInterruptType intChannel;
} ArduinoPinDescriptionType;
```

We have an array of these called **`g_ArduinoPinDescription[]`** defined in [pins_arduino.cpp](https://github.com/zeeduino/zeeduino-xc-hal/blob/master/src/pins_arduino.cpp#L97).

Each element of this array corresponds to a pin in Arduino context. For example, **Arduino pin 7** is described by the element **`g_ArduinoPinDescription[7]`**.

> **This array is essentially a map between Arduino pins and chip's physical port/pin combination.**
> 
> **It also contains some instructions on how to configure port/pin on the chip to do what we need it to do for Arduino pin that maps to it.**

Each `g_ArduinoPinDescription` element has:

- **port**: this is microcontroller port where this Arduino pin is
- **pin**: this is *microcontroller pin* at above specified port, where this Arduino pin is
- **modefunc**: this is 16-bit value used to set up the chip pin to provide certain function. It is up to you to use this to store what you need to accomplish that task
- **adcChannel**: this is Arduino ADC channel number this Arduino pin is assigned to
- **pwmChannel**: this is Arduino PWM channel number this Arduino pin is assigned to
- **intChannel**: this is Arduino interrupt channel number this Arduino pin is assigned to

Similar goes for the second most important type: **`DigitalMappingType`**:

```C++
typedef struct _DigitalMappingType
{
    LPC_GPIO_T *pPORT;
    uint32_t modefunc:16;
    uint32_t modefuncPullup:16;
    uint32_t modefuncPulldown:16;
    uint32_t reserved:16;
} DigitalMappingType;
```

We have an array of these called **`g_ArduinoPinMappingDigital[]`** which contains additional information needed by HAL when an Arduino pin is set to be of **digital** type.

There is also the **`g_ArduinoPinMappingPwm[]`** array which contains information needed by HAL when an Arduino pin is set to be **analog output**. You may or may not need this array, depending on what capabilities you want to implement in your HAL.


Your task is to:

- decide how many pins of your chip you want to use as Arduino pins
- fill in elements of the `g_ArduinoPinDescription` array for chosen Arduino pins with correct values
- fill in elements of the `g_ArduinoPinMappingDigital` array for chosen Arduino pins with correct values
- fill in elements of the `g_ArduinoPinMappingPwm` array for chosen PWM channels and Arduino pins with correct values
- mark all the rest of elements with port number value **`99`** and **`NO_`** constants
- add chip specific code to **`Variant_Pins_Init()`** function to initialize microcontroller's physical pins with values configured in `g_ArduinoPinDescription` array, plus any other low-level pin related initialization (i.e. USB pins, JTAG pins etc.)


As a first step, we will have two pins on our custom Zeeduino-XC board:

- LED pin - pin #13
- additional pin to play with - pin #3

```C++
const ArduinoPinDescriptionType g_ArduinoPinDescription[] =
{
        // 0 - 1, UART
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC

        // 2 - 12, Digital pins
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 1, 15, (IOCON_FUNC0 | IOCON_MODE_PULLDOWN), NO_ADC, PWM_5, EXT_INT_1 }, // 3 - PWM, 16B0_MAT2
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC
        { 99, 99, (0), NO_ADC, NOT_ON_PWM, NO_EXT_INT }, // FUNC

        // 13, default LED
        { 0,  7,  (IOCON_FUNC0 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT), NO_ADC, NOT_ON_PWM, NO_EXT_INT },  // 13 - LED
};

const DigitalMappingType g_ArduinoPinMappingDigital[] =
{
        // 0 - 1, UART
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },

        // 2 - 12, Digital pins
        { NULL, (0), (0), (0), (0) },
        { LPC_GPIO_PORT, (IOCON_FUNC0), (IOCON_MODE_PULLUP), (IOCON_MODE_PULLDOWN), (0) }, // 1.15
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },
        { NULL, (0), (0), (0), (0) },

        // 13, default LED
        { LPC_GPIO_PORT, (IOCON_FUNC0), (IOCON_MODE_PULLUP), (IOCON_MODE_PULLDOWN), (0) }, // 0.7
};
```

Notice how all the pins, except for #3 and #13, are marked with **`NO_`** constants and **`(0)`**'s. These will be skipped in initialization and later on, HAL functions will take care of doing nothing if pin you use in your Arduino sketch is not implemented.

> If Arduino pin is implemented or not is determined by having value of **`99`** for `port` field in `g_ArduinoPinDescription` array element corresponding to that Arduino pin.

Also, notice that:

> NOTE 1

In our example above, we used **`IOCON_`** constants for `modefunc` fields. These are specific to LPC13xx family of microcontrollers. Values stored in these fields are directly used in calls to the LPCOpen chip support library. Depending on how chip support library works for your chosen microcontroller, you can:

- store library function call parameters in `modefunc` fields directly,
- or you can come up with constants of your own and then do different things to your chip based off of what value you find in `modefunc` field for that Arduino pin.

> NOTE 2

`DigitalMappingType` and `PwmMappingType` have `void *` members `pPORT` and `pTMR`. You can use these either as pointers or 32-bit values. These uniquely identify a port or a timer/pwm peripheral on the chip. For LPC13xx implementation we used these as pointers and did cast to appropriate pointers to chip-specific structures in **`DIG_MAP_PORT`** and **`PWM_MAP_TIMER`** macros. You need to change these casts to whatever fits your implementation:

- either cast to a pointers to chip specific structures or
- cast to 32-bit value.

Now that you have set up pin mapping for your chip, it's time to implement digital pin functions.


##### Implementing digital pin functions

So, to recap:

- your IDE has generated boilerplate code to reset and initialize chip and C++ stack
- you have added call to `Board_SystemInit()` at an appropriate place in the boilerplate code
- you removed generic `main()` function from the generated project
- pin mapping is implemented for pins #3 and #13
- pin intialization function `Variant_Pins_Init()` executes pin initalization based off of what you have put in `g_ArduinoPinDescription[]` array

Now you need to implement three HAL functions in order to be able to work with digital pins in Zeeduino-XC.

In [`src/board/w_digital.cpp`](https://github.com/zeeduino/Zeeduino-XC-portbase/blob/master/src/board/w_digital.cpp) file you have to add some code to these functions:

- `Board_Digital_PinMode()`
- `Board_Digital_Write()`
- `Board_Digital_Read()`

Each of these has a part of it's code with the comment which starts like this:

```C++
/* call chip library/your code here to ... */
```

Your task is to add/change/replace whatever is immediately below that comment with actual calls to functions from your chip's support library in order to do the task that HAL function requires:

- `Board_Digital_PinMode()`: set the physical port/pin mode to digital
- `Board_Digital_Write()`: write a value to selected port/pin
- `Board_Digital_Read()`: read a value from selected port/pin

[LPCOpen] chip support library provides one function for each of these tasks and it should be similar for other chips and their support libraries. We used `port`, `pin`, `pPORT` and `modefunc` values to call these LPCOpen functions with correct parameters. Do something similar with functions for your chip.

Once done, rebuild your Zeeduino-XC-portbase library and make sure all errors and warnings are gone.


##### Implementing delay functions

In order to be able to blink a LED using only Arduino code, we also need to implement HAL functions which support Arduino delay functions.

This should be fairly simple, if your chip has dedicated timer peripheral, which most of today's chips should have.

The timer should be able to count microseconds, milliseconds in worst case, in order to be usable for Arduino. File [`src/board/w_delay.cpp`](https://github.com/zeeduino/Zeeduino-XC-portbase/blob/master/src/board/w_delay.cpp) contains all the HAL functions that need to be implemented for Arduino delay/timing support.

First functions you need to implement is the one that initializes the timer: **`Board_Delay_InitTimer()`**. Just add your timer initialization chip specific code below the comment:

```C++
/* call chip library/your code here to initialize chip hardware timer/counter */
```

Also, you have to set correct value for `ticksPerSecond` variable:

```C++
ticksPerSecond = 1000000/* this number is chip/board specific */;
```

Once you've done that, the next ones to implement are:

- **`Board_Delay_Micros()`**
- **`Board_Delay_Millis()`**

Both of these need just one thing: call to chip library support function that reads the timer/counter current value. Call to this function is simulated as a call to non-existing `read_timer_count_from_chip_timer_counter()` function. Replace this function with call to the correct one or write your own code to read timer/counter current value.

Having finished that, you now have working Arduino delay related functions (delay(), millis() etc.).

**!!! IMPORTANT NOTE ABOUT DELAY FUNCTIONS !!!**
> Since timer/counter is 32-bit register/value (usually), it will wrap around fairly soon after chip powers up. This means that, i.e. using `millis()` function to implement your own non-blocking delays will fail after a while unless you handle this wrap-around. This wrap-around happens after ~477 seconds in our Zeeduino-XC-lpc13xx HAL implementation.
> 
> [basicSketch.cpp] from [basicSketch project] contains implementation of `timeLapseMillis()` function which handles wrap-around, and which you can use to check if the time has come to do some periodic work, like blinking a LED.
> 
> Commented out code in [basicSketch.cpp] is the way that you should not do this, it will fail after some time in a sense that `if` condition will always be false.

#### Running first Arduino sketch on your microcontroller (finally!!)

Now you can rebuild basicSketch project you previously created and it will link against your portbase library which has digital pin and delay **`Board*`** functions implemented. If you connect LED to whatever pin on your chip that you decided will be the Arduino pin #13 (default LED pin), you should see the LED blinking when you run the basicSketch project.

Hooray!!


#### Implementing other HAL functions

It would take an entire book to write in order to go through all the details on how to implement the rest of the HAL functions.

Luckily, there's at least one complete implementation of all the HAL functions listed in the next section. You can take a look at [Zeeduino-XC HAL implementation for LPC13xx](https://github.com/zeeduino/zeeduino-xc-hal-lpc13xx), see how various board functions are implemented there and do similar thing for your microcontroller.

You have to do that only once, and after that every next Arduino project on your microcontroller will be a breeze to start and to code!

Enjoy!

#### List of HAL functions

Initialization:

* `src/variant.cpp`
    - **`initVariant()`**
* `src/pins_arduino.cpp`
    - **`Variant_Pins_Init()`**
* `src/board/board_init.cpp`
    - **`Board_Init()`**

Private:

* `src/board/board_private.c`
    - **`whatever funstions are just for use within HAL library()`**

Core functions:

* `src/board/w_delay.cpp`
    - **`Board_Delay_Millis()`**
    - **`Board_Delay_Micros()`**
    - **`Board_Delay_MicrosMax()`**
    - **`Board_Delay_InitTimer()`**
* `src/board/w_digital.cpp`
    - **`Board_Digital_PinMode()`**
    - **`Board_Digital_Write()`**
    - **`Board_Digital_Read()`**
* `src/board/w_analog.cpp`
    - **`Board_Analog_Read()`**
    - **`Board_Analog_Write()`**
* `src/board/w_interrupts.cpp`
    - **`PIN_INTnn_IRQHandler()`** (private, one for each interrupt channel)
    - **`Board_Attach_Interrupt()`**
    - **`Board_Detach_Interrupt()`**

Core library (Serial, SPI, Wire):

* `src/board/board_serial.cpp`
    - **`Serial_Init()`**
    - **`Serial_UART_Init()`**
    - **`Serial_UART_End()`**
    - **`Serial_UART_Flush()`**
    - **`Serial_UART_Transmit()`**
    - **`Serial_UART_Receive()`**
    - **`Serial_UART_TxRegisterEmpty()`**
    - **`Serial_UART_RxRegisterHasData()`**
    - **`Serial_UART_Set_Interrupt_Priority()`**
    - **`Serial_UART_Get_Interrupt_Priority()`**
    - **`Serial_UART_Disable_Interrupt()`**
    - **`Serial_UART_Enable_Interrupt()`**
    - **`UART_IRQHandler()`** (private, depends on how/if UART interrupts are handled on your chip)
* `src/board/board_spi.cpp`
    - **`#define CS_LOW()`**
    - **`#define CS_HIGH()`**
    - **`#define SCK_LOW()`**
    - **`#define SCK_HIGH()`**
    - **`#define MISO()`**
    - **`#define MOSI()`**
    - **`Board_SPI_SetInternalPortStateDefault()`** (used internally only)
    - **`Board_SPI_SetInternalPortState()`** (used internally only)
    - **`Board_SPI_Transfer()`**
    - **`Board_SPI_CS_High()`**
    - **`Board_SPI_CS_Low()`**
    - **`Board_SPI_Init()`**
    - **`Board_SPI_End()`**
* `src/board/board_wire.cpp`
    - **`Board_I2C_Master_Init()`**
    - **`Board_I2C_Slave_Init()`**
    - **`Board_I2C_Set_Bus_Speed()`**
    - **`Board_I2C_Master_Read_Blocking()`**
    - **`Board_I2C_Master_Write_Blocking()`**
    - **`WIRE_ISR_HANDLER()`** (private, depends on how/if SPI interrupts are handled on your chip)




[LPCOpen]: http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/lpc-cortex-m-mcus/software-tools/lpcopen-libraries-and-examples:LPC-OPEN-LIBRARIES
[Atmel Software Framework]: http://www.atmel.com/tools/avrsoftwareframework.aspx
[basicSketch project]: https://github.com/zeeduino/zeeduino-xc-basicSketch
[basicSketch.cpp]: https://github.com/zeeduino/zeeduino-xc-basicSketch/blob/master/src/basicSketch.cpp

[Zeeduino-XC library on lpc1347]: http://zeeduino.com/lpc1347-videos
[creating portbase library for lpc11u37]: http://zeeduino.com/how
