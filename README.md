# LibRfsAvr

## Overview

`LibRfsAvr` is a library that creates an abstraction layer to the AVR microcontroller features and subsystems. It is built on top of `avr-glibc`. It's purpose is to allow to program AVR microcontrollers by writting more readable and maintainable code, and thus speeding up the learning process.

This library has to main characteristics:
* *No use of interrupts*: Instead, the library polls the different flags in order to know when the processes have finished. Interrupts introduce overhead (context change) and complexifies the code, as sometimes interrupts have to be disabled and re-enabled.

* *Non-blocking routines and asynchronous code*: The routines that this library offer are very fast and never block. Instead, all the processes are performed asynchronously. The recommended way of using this library is by writing code that uses states to track the state of the different processes. For example:

```c
int main()
{
    int8_t state = IDLE;

    // Main loop (loop forever)
    do {
        switch (state) {
        case IDLE:
            start_something();
            state = WORKING;
            break;
        case WORKING:
            if (something_finished()) {
                state = IDLE;
            }
            break;
        }
    } while (1);
}
```

In this case, `start_something()` is a non-blocking function that starts some process and returns. The process progresses in the background (usually some hardware peripheral doing something), and the `something_finished()` call is used to poll the process whether it has finished. Several of these state machines can be executed in series to manage several process concurrently.

## Build and installation

The library can be installed like any other autotools-managed package:

```bash
./configure
make
make install
```

## Using the library

The library has to be linked by adding the `-lrfsavr` flag to the linker:

```bash
avr-gcc -mmcu=${MCU} -o myprogram myprogram.o -lrfsavr
```

## Subsystems

### PWM

