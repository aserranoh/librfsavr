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
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER0);
    uint8_t size = sprintf(buffer, "1:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p\n", pwm.timer.cra, &TCCR0A, pwm.timer.crb, &TCCR0B, pwm.timer.cnt, &TCNT0,
        pwm.timer.ocra, &OCR0A, pwm.timer.ocrb, &OCR0B, pwm.timer.ifr, &TIFR0);
    write_result(buffer, size);
}

void test_init_timer2()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER2);
    uint8_t size = sprintf(buffer, "2:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p\n", pwm.timer.cra, &TCCR2A, pwm.timer.crb, &TCCR2B, pwm.timer.cnt, &TCNT2,
        pwm.timer.ocra, &OCR2A, pwm.timer.ocrb, &OCR2B, pwm.timer.ifr, &TIFR2);
    write_result(buffer, size);
}

void test_enable_channel_A_timer0()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER0);
    rfs_pwm8_enable_channel_A(&pwm);
    uint8_t size = sprintf(buffer, "3:%hhx,%hhx\n", TCCR0A, DDRD);
    write_result(buffer, size);
}

void test_enable_channel_B_timer0()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER0);
    rfs_pwm8_enable_channel_B(&pwm);
    uint8_t size = sprintf(buffer, "4:%hhx,%hhx\n", TCCR0A, DDRD);
    write_result(buffer, size);
}

void test_enable_channel_A_timer2()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER2);
    rfs_pwm8_enable_channel_A(&pwm);
    uint8_t size = sprintf(buffer, "5:%hhx,%hhx\n", TCCR2A, DDRB);
    write_result(buffer, size);
}

void test_enable_channel_B_timer2()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER2);
    rfs_pwm8_enable_channel_B(&pwm);
    uint8_t size = sprintf(buffer, "6:%hhx,%hhx\n", TCCR2A, DDRD);
    write_result(buffer, size);
}

void test_disable_channel_A_timer0()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER0);
    rfs_pwm8_disable_channel_A(&pwm);
    uint8_t size = sprintf(buffer, "7:%hhx\n", TCCR0A);
    write_result(buffer, size);
}

void test_disable_channel_B_timer0()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER0);
    rfs_pwm8_disable_channel_B(&pwm);
    uint8_t size = sprintf(buffer, "8:%hhx\n", TCCR0A);
    write_result(buffer, size);
}

void test_disable_channel_A_timer2()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER2);
    rfs_pwm8_disable_channel_A(&pwm);
    uint8_t size = sprintf(buffer, "9:%hhx\n", TCCR2A);
    write_result(buffer, size);
}

void test_disable_channel_B_timer2()
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER2);
    rfs_pwm8_disable_channel_B(&pwm);
    uint8_t size = sprintf(buffer, "10:%hhx\n", TCCR2A);
    write_result(buffer, size);
}

void test_set_frequency_timer0(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER0);
    rfs_pwm8_set_frequency(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx\n", test_id, TCCR0A, TCCR0B);
    write_result(buffer, size);
}

void test_set_frequency_timer2(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER2);
    rfs_pwm8_set_frequency(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx\n", test_id, TCCR2A, TCCR2B);
    write_result(buffer, size);
}

void test_set_frequency_exact_timer0(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm8_t pwm;
    rfs_pwm8_init(&pwm, RFS_TIMER0);
    rfs_pwm8_set_frequency_exact(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx,%hhx\n", test_id, TCCR0A, TCCR0B, OCR0A);
    write_result(buffer, size);
}

int main()
{
    init_usart();

    test_init_timer0();
    test_init_timer2();
    test_enable_channel_A_timer0();
    test_enable_channel_B_timer0();
    test_enable_channel_A_timer2();
    test_enable_channel_B_timer2();
    test_disable_channel_A_timer0();
    test_disable_channel_B_timer0();
    test_disable_channel_A_timer2();
    test_disable_channel_B_timer2();
    test_set_frequency_timer0(11, 63000);
    test_set_frequency_timer0(12, 62500);
    test_set_frequency_timer0(13, 31251);
    test_set_frequency_timer0(14, 31250);
    test_set_frequency_timer0(15, 7813);
    test_set_frequency_timer0(16, 7812);
    test_set_frequency_timer0(17, 3907);
    test_set_frequency_timer0(18, 3906);
    test_set_frequency_timer0(19, 977);
    test_set_frequency_timer0(20, 976);
    test_set_frequency_timer0(21, 489);
    test_set_frequency_timer0(22, 488);
    test_set_frequency_timer0(23, 245);
    test_set_frequency_timer0(24, 244);
    test_set_frequency_timer0(25, 123);
    test_set_frequency_timer0(26, 122);
    test_set_frequency_timer0(27, 62);
    test_set_frequency_timer0(28, 61);
    test_set_frequency_timer0(29, 31);
    test_set_frequency_timer0(30, 30);
    test_set_frequency_timer2(31, 63000);
    test_set_frequency_timer2(32, 62500);
    test_set_frequency_timer2(33, 31251);
    test_set_frequency_timer2(34, 31250);
    test_set_frequency_timer2(35, 7813);
    test_set_frequency_timer2(36, 7812);
    test_set_frequency_timer2(37, 3907);
    test_set_frequency_timer2(38, 3906);
    test_set_frequency_timer2(39, 1954);
    test_set_frequency_timer2(40, 1953);
    test_set_frequency_timer2(41, 977);
    test_set_frequency_timer2(42, 976);
    test_set_frequency_timer2(43, 489);
    test_set_frequency_timer2(44, 488);
    test_set_frequency_timer2(45, 245);
    test_set_frequency_timer2(46, 244);
    test_set_frequency_timer2(47, 123);
    test_set_frequency_timer2(48, 122);
    test_set_frequency_timer2(49, 62);
    test_set_frequency_timer2(50, 61);
    test_set_frequency_timer2(51, 31);
    test_set_frequency_timer2(52, 30);
    test_set_frequency_exact_timer0(53, 50000);

    // Loop forever
    while (1);
}