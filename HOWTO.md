## Arduino-XC on your microcontroller



### Using the library with already implemented HAL

Check the [LPC1347 Arduino-XC HAL implementation](https://github.com/zeeduino/arduino-xc-lpc13xx) for example of using already implemented
HAL in your project.

In this case it is implementation for NXP LPC1347 micro which is then used with LPCXpresso IDE to create and run Arduino-XC projects.



### Implementing the HAL yourself

> If you are contemplating of implementing HAL layer for your favourite microcontroller yourself, keep in mind that this is not a novice exercise, you should have sufficient knowledge and experience building projects and writing code for your controller before you start. Because hardly any coding ever goes without hiccups and this one is bound to get you to a hard spot or two until the initial HAL implementation is done and working.
> 
> After that, it is a smooth ride, having Arduino code running on your chip.

In order for Arduino-XC to work on your microcontroler, you will have to provide pieces of code which are specific to your chosen chip.

Process is actually fairly simple and can be done in steps where after each step you'll have something to play with.

In order to help you start faster, there are two projects you can use:

- Arduino-XC HAL porting base code: https://github.com/zeeduino/arduino-xc-portbase
- basic sketch project for LPCXpresso IDE: https://github.com/zeeduino/arduino-xc-basicSketch

The portbase project contains all the source files you'll need to make Arduino-XC work on your microcontroller.

In order to have HAL implemented for your controller, you'll have to:

- prepare the environment in your development IDE for working on HAL implementation
    * build arduino-xc library
    * download and build chip specific support libraries
    * create and build initial dummy HAL implementation project
    * create basic sketch project
- implement HAL in 4 steps
    * setup pinout
    * implement digital functions
    * implement serial object
    * implement other pieces when you need them

So, let's start!


#### Arduino-XC library

First thing to do is to create a C++ library project using your chip's development environment and [arduino-xc source](https://github.com/zeeduino/arduino-xc). Check [this video][arduino-xc library on lpc1347] to see how we did it for LPC1347 and LPCXpresso IDE.

Arduino-XC core code is microcontroller-agnostic and should compile with no issues on any standard C++ compiler.

Once Arduino-XC core is compiled and you have library built, it's time to prepare for implementing HAL.


#### Chip-specific support libraries

These days, every microcontroller manufacturer has some form of basic support code available for their chips. This is usually in a form of a library like [LPCOpen] for LPC controllers or [Atmel Software Framework] for AVR and SAM controllers, etc. Or it may be a bunch of application notes and some source code and examples to get you started quickly with that chip.

Whatever is the case, you should get those libraries (or library) for your chosen chip and build and test them with simple blinky led program.

Once you have your chip's support library in place and working, it's time to create Arduino-XC HAL implementation library for your microcontroller.


#### HAL implementation library

Clone the [portbase code](https://github.com/zeeduino/arduino-xc-portbase) and rename it to something that will be specific to your microcontroller, i.e. arduino-xc-lpc13xx.

Now you need to create a C++ library project using your chip's development environment and add portbase source to it. You will also have to add references to arduino-xc library to the portbase project. Check out [this video][creating portbase library for lpc11u37] how we did it for LPC11U37 using LPCXpresso IDE. It will certainly be different for your controller, but it helps to get the general idea of what you need to do.

The most imprtant thing is that arduino-xc include folder is referenced in your portbase project.

Next important thing is that your chip support library (LPCOpen, Atmel Software Framework, etc.) is referenced by your portbase project.

Those two important things kind of underline the fact that HAL implementation library is a bridge between Arduino-XC core code and the chip specific code.

Having your portbase project created and set up, build it. It will probably contain few errors or warnings, like complaining about undefined function or two. This is expected because arduino-xc-portbase project contains couple of calls to functions which do not exist and are there just as a placeholder or reminder. For example, **read_the_char_from_UART()** function is not a function, it just states that you need to read a character from your chip's UART peripheral at this point.

You can comment out these places where you have errors or warnings and aim to have your portbase compile clean, with no errors and warnings.

Once the portbase project is set up and ready, it's time to create a basic Arduino sketch we will use to check our HAL implementation progress.


#### Basic Arduino sketch project

You can find a source code for basic sketch [here][basicSketch project]. However, that project is specific for LPC13xx controllers and the only thing you'll use from there is [basicSketch.cpp](https://github.com/zeeduino/arduino-xc-basicSketch/blob/master/src/basicSketch.cpp) file. We'll need other files just as an example of how to get the basic sketch working in your project.

As a quick start help, you can also use **`src/board.c`** and **`inc/board.h`** files and copy them into your project.

Create an empty C++ project using your chip's development environment (for short, we'll call it IDE from now on) and add basicSketch.cpp to it. Your IDE should have created a source file which contains **main()** function. If that file only contains the main() function and nothing else of importance, you should delete that file. If the file contains some important bits and pieces, like chip initialization code, you should just delete the **main()** function and leave the file in the project.

Next, add references to the project to:
- arduino-xc library
- your copy of arduino-xc-portbase library
- your chip's support library

Order in which these are referenced in the project (not added to the project, but referenced!) might be important (for example for the linker to find all the symbols), and the one above is usually working ok.

Now we need to tie in few pieces to get this Frankenstein off the ground.


##### Chip initialization code

There usually is some sort of chip initialization code that starts up the chip, does some basic setup and then calls the **main()** function. [basicSketch project] contains `src` folder and within that folder there's a `lpcx` folder. the `lpcx` contains all the code that LPCXpressi IDE auto-generated when we created a C++ project.

You may or may not want to look at those files, their content is not relevant for our work here, except for a few pieces.

###### Call Board_SystemInit()

[sysinit.c](https://github.com/zeeduino/arduino-xc-basicSketch/blob/master/src/lpcx/sysinit.c) file contains `SystemInit()` function which calls **`Board_SystemInit()`**. Find a similar place in the code generated by your IDE and place the call to `Board_SystemInit()` there.

Tha main thing `Board_SystemInit()` has to do is to call **`initVariant()`** function. `Board_SystemInit()` seems like a unnecessary wrapper to call `initVariant()` but we did it this way so you can do some application specific initialization in `Board_SystemInit()`.

If you do that, however, keep in mind that your Arduino code will not be easily portable anymore. On the other hand, Arduino-XC is here to enable you to quickly start with a project on your microcontroller, test how it works, and then add application specific detail which do not have to be portable, they only have to work and do what you need them to do in your hardware project.


###### Make sure `main()` is called from somewhere

Depending on your IDE, this may or may not be obvius. You just have to make sure that you didn't remove the code that calls `main()` from your project. In our case of [basicSketch project]





[LPCOpen]: http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/lpc-cortex-m-mcus/software-tools/lpcopen-libraries-and-examples:LPC-OPEN-LIBRARIES
[Atmel Software Framework]: http://www.atmel.com/tools/avrsoftwareframework.aspx
[arduino-xc library on lpc1347]: http://zeeduino.com
[basicSketch project]: https://github.com/zeeduino/arduino-xc-basicSketch
