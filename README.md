# LibRfsAvr

## Overview

`LibRfsAvr` is a library that creates an abstraction layer to the AVR microcontroller features and subsystems. It is built on top of `avr-glibc`. It's purpose is to allow to program AVR microcontrollers by writting more readable and maintainable code, and thus speeding up the learning process.

This library has two main characteristics:
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

In this case, `start_something()` is a non-blocking function that starts some process and returns. The process progresses in the background (usually some hardware peripheral doing something), and the `something_finished()` non-blocking call is used to poll the process whether it has finished. Several of these state machines can be maintained to manage several processes concurrently.

## Build and installation

The library can be installed like any other autotools-managed package:

```bash
./configure
make
make install
```

The result is actually no one single library, but one library per AVR sub-architecture. For now, only the ATMega328P sub-architecture is supported.

## Using the library

The library has to be linked by adding the `-lrfsavr-<mmcu>` flag to the linker, specifying the right library to use. For example, for the ATMega328P sub-architecture, the linker call would be:

```bash
MCU="atmega328p" avr-gcc -mmcu=${MCU} -o myprogram myprogram.o -lrfsavr-${MCU}
```

## Subsystems

### PWM

The library offers types and functions to generate PWM signals using the microcontroller. For that, the library defines the `struct rfs_pwm_t` type. In order to use the PWM signal, this structure has to be first initialized, using the function:

```c
#include <rfs/pwm.h>

void rfs_pwm_init(struct rfs_pwm_t *pwm,
                  enum rfs_timer_enum timer,
                  enum rfs_pwm_channel channel);
```

The PWM subsystem is built on top of the Timer subsystem, since the AVR timers are the responsible of generating the PWM signals. The parameter `timer` specifies which of the microcontroller's timers to use in order to generate the PWM signal. The enumerate defines constants of the type `RFS_TIMERX`, where `X` is the timer number (0, 1, etc.). Be aware that the microcontrollers have timers of different resolutions (8 and 16 bits), and thus the generated PWM signal will have that resolution, depending on the chosen timer.

Finally, there's the channel parameter. The microcontroller's timers are able to generate PWM signals on two channels (A and B). So, the channel parameter specifies which channel will be used for the PWM signal. The possible values are `RFS_PWM_CHANNEL_A` and `RFS_PWM_CHANNEL_B`. Depending on the timer and the channel chosen, the PWM signal is physically output on a specific pin. The following table shows the output pin as a function of the timer and channel for the ATMega328P microcontroller:

| Timer   | Channel   | Output pin |
| ------- | :-------: | :--------: |
| Timer 0 | Channel A | PD6        |
| Timer 0 | Channel B | PD5        |
| Timer 1 | Channel A | PB1        |
| Timer 1 | Channel B | PB2        |
| Timer 2 | Channel A | PB3        |
| Timer 2 | Channel B | PD3        |

After the `struct rfs_pwm_t` type has been initialized, we have to configure the frequency of the PWM signal. For this, there's actually two possible functions:

```c
void rfs_pwm_set_frequency(const struct rfs_pwm_t *pwm,
                           uint32_t frequency,
                           uint32_t cpu_frequency);

void rfs_pwm_set_frequency_hint(const struct rfs_pwm_t *pwm,
                                uint32_t frequency,
                                uint32_t cpu_frequency);
```

Both functions take the same parameters: an initialized instance of a `struct rfs_pwm_t` type, the target frequency that we want to configure the PWM signal with, and the CPU frequency. The CPU frequency is necessary because the PWM frequency is usually configured as the CPU frequency divided by a given factor. The CPU frequency is usually passed to the program as a `define`.

The `rfs_pwm_set_frequency_hint` sets the PWM frequency to the closest to the target frequency that can be reached with the microcontroller CPU frequency and the available clock divisors. The used frequency will be always lower than the target frequency. The following table shows the actual PWM frequency that will be used for different target frequency intervals, as a function of the Top PWM frequency (F), which is CPU_freq/256, for the ATMega328P and Timer 0:

| Target frequency interval (Hz) | Used frequency (Hz) |
| ------------------------------ | :-----------------: |
| (F, 0xffffffff]                | F                   |
| (F/2, F]                       | F/2                 |
| (F/8, F/2]                     | F/8                 |
| (F/16, F/8]                    | F/16                |
| (F/64, F/16]                   | F/64                |
| (F/128, F/64]                  | F/128               |
| (F/256, F/128]                 | F/256               |
| (F/512, F/256]                 | F/512               |
| (F/1024, F/512]                | F/1024              |
| [1, F/1024]                    | F/2048              |

The `rfs_pwm_set_frequency` function tries to set an exact frequency to the PWM signal, but this is done by sacrifycing some resolution for the duty cycle. In fact, by adjusting the TOP value that the timer uses to generate the PWM signal, a faster frequency can be reached.

> [!NOTE]
> When using 8-bit timers, if an exact value for the frequency is used, only the Channel B of that timer can be used.

`rfs_pwm_set_frequency_hint` is good for applications where the exact frequency is not that important, like when controlling DC motors, where the PWM frequency is not important as long as it is high enough, and `rfs_pwm_set_frequency` when the device protocol enforces a PWM signal of an exact frequency, like the servos.

> [!NOTE]
> The two channels of the same timer use the same frequency. So, if we have two PWM instances, one for each channel of the same timer, changing the frequency in one of them will affect the frequency on the other as well.

The following functions are used to set the PWM signal's duty cycle:

```c
void rfs_pwm_set_duty_cycle_8(const struct rfs_pwm_t *pwm,
                              uint8_t duty_cycle);

void rfs_pwm_set_duty_cycle_16(const struct rfs_pwm_t *pwm,
                               uint16_t duty_cycle);
```

The `rfs_pwm_set_duty_cycle_8` function has to be used when using an 8-bit timer to generate the PWM signal and the `rfs_pwm_set_duty_cycle_16` when a 16-bit timer is being used.

Finally, if the PWM output is not used anymore, the following function can be called in order to deactivate the signal and save some power:

```c
void rfs_pwm_close(const struct rfs_pwm_t *pwm);
```

The following example shows how to configure a PWM signal with the purpose of driving a DC motor, using an 8-bit timer. Then, it sets the duty cycle of the signal to 50%:

```c
#include <rfs/pwm.h>

struct rfs_pwm_t pwm;

void initialize()
{
    // Initialize the PWM signal, using timer 0 and channel A
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_A);

    // Set a frequency close to 50 kHz
    rfs_pwm_set_frequency_hint(&pwm, 50000, F_CPU);
}

void process()
{
    // Set the duty cycle to 50%
    rfs_pwm_set_duty_cycle_8(&pwm, 128);
}

int main()
{
    initialize();
    do {
        process();
    } while (1);
}
```

The following example shows how to configure a PWM signal to operate a servo, using a 16-bit timer. The servo operates at 50 Hz (period of 20 ms), and it is set at 90º (duty cycle of 1.5 ms):

```c
#include <rfs/pwm.h>

#define DEG_90  (1.5 * 65536 / 20)

struct rfs_pwm_t pwm;

void initialize()
{
    // Initialize the PWM signal, using timer 1 (16 bit) and channel A
    rfs_pwm_init(&pwm, RFS_TIMER1, RFS_PWM_CHANNEL_A);

    // Set an exact frequency of 50 Hz
    rfs_pwm_set_frequency(&pwm, 50, F_CPU);
}

void process()
{
    // Set the duty cycle to 1.5 ms
    rfs_pwm_set_duty_cycle_16(&pwm, DEG_90);
}

int main()
{
    initialize();
    do {
        process();
    } while (1);
}
```