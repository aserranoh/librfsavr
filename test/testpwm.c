/*
testpwm.c - Test program for the PWM subsystem.

This file is part of RobotsFromScratch.

Copyright 2023 Antonio Serrano Hernandez

RobotsFromScratch is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RobotsFromScratch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RobotsFromScratch; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.
*/

#include "rfsavr/pwm.h"
#include "rfsavr/usart.h"
#include <avr/io.h>
#include <stdio.h>

struct rfs_usart_t usart;
struct rfs_pwm8_t pwm;
char buffer[64];

void init_usart()
{
    rfs_usart_open(&usart, RFS_USART_0, RFS_USART_ASYNC, RFS_USART_RXTX | RFS_USART_8BITS);
    rfs_usart_setspeed(&usart, RFS_USART_B19200, F_CPU);
}

void write_result(char *buffer, uint8_t size)
{
    uint8_t written = 0;
    char *current_ptr = buffer;
    while (written < size) {
        if (rfs_usart_write(&usart, *current_ptr)) {
            written++;
            current_ptr++;
        }
    }
}

void test_init_timer0()
{
    rfs_pwm8_init(&pwm, RFS_TIMER0);
    uint8_t size = sprintf(buffer, "1:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p\n", pwm.timer.cra, &TCCR0A, pwm.timer.crb, &TCCR0B, pwm.timer.cnt, &TCNT0,
        pwm.timer.ocra, &OCR0A, pwm.timer.ocrb, &OCR0B, pwm.timer.ifr, &TIFR0);
    write_result(buffer, size);
}

void test_init_timer2()
{
    rfs_pwm8_init(&pwm, RFS_TIMER2);
    uint8_t size = sprintf(buffer, "2:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p\n", pwm.timer.cra, &TCCR2A, pwm.timer.crb, &TCCR2B, pwm.timer.cnt, &TCNT2,
        pwm.timer.ocra, &OCR2A, pwm.timer.ocrb, &OCR2B, pwm.timer.ifr, &TIFR2);
    write_result(buffer, size);
}

int main()
{
    init_usart();

    test_init_timer0();
    test_init_timer2();

    // Loop forever
    while (1);
}