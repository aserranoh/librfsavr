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

void test_init_timer0_a()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_A);
    uint8_t size = sprintf(buffer, "1:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%hhx,%hhx,%hhx\n", pwm.timer.cra, &TCCR0A, pwm.timer.ocra, &OCR0A, pwm.timer.ocrb, &OCR0B,
        pwm.pin->port, &PORTD, pwm.ocr, &OCR0A, pwm.channel, pwm.pin->pin, TCCR0A);
    write_result(buffer, size);
}

void test_init_timer0_b()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_B);
    uint8_t size = sprintf(buffer, "2:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%hhx,%hhx,%hhx\n", pwm.timer.cra, &TCCR0A, pwm.timer.ocra, &OCR0A, pwm.timer.ocrb, &OCR0B,
        pwm.pin->port, &PORTD, pwm.ocr, &OCR0B, pwm.channel, pwm.pin->pin, TCCR0A);
    write_result(buffer, size);
}

void test_init_timer1_a()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER1, RFS_PWM_CHANNEL_A);
    uint8_t size = sprintf(buffer, "3:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%hhx,%hhx,%hhx\n", pwm.timer.cra, &TCCR1A, pwm.timer.ocra, &OCR1AL, pwm.timer.ocrb, &OCR1BL,
        pwm.pin->port, &PORTB, pwm.ocr, &OCR1AL, pwm.channel, pwm.pin->pin, TCCR1A);
    write_result(buffer, size);
}

void test_init_timer1_b()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER1, RFS_PWM_CHANNEL_B);
    uint8_t size = sprintf(buffer, "4:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%hhx,%hhx,%hhx\n", pwm.timer.cra, &TCCR1A, pwm.timer.ocra, &OCR1AL, pwm.timer.ocrb, &OCR1BL,
        pwm.pin->port, &PORTB, pwm.ocr, &OCR1BL, pwm.channel, pwm.pin->pin, TCCR1A);
    write_result(buffer, size);
}

void test_init_timer2_a()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2, RFS_PWM_CHANNEL_A);
    uint8_t size = sprintf(buffer, "5:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%hhx,%hhx,%hhx\n", pwm.timer.cra, &TCCR2A, pwm.timer.ocra, &OCR2A, pwm.timer.ocrb, &OCR2B,
        pwm.pin->port, &PORTB, pwm.ocr, &OCR2A, pwm.channel, pwm.pin->pin, TCCR2A);
    write_result(buffer, size);
}

void test_init_timer2_b()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2, RFS_PWM_CHANNEL_B);
    uint8_t size = sprintf(buffer, "6:%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%hhx,%hhx,%hhx\n", pwm.timer.cra, &TCCR2A, pwm.timer.ocra, &OCR2A, pwm.timer.ocrb, &OCR2B,
        pwm.pin->port, &PORTD, pwm.ocr, &OCR2B, pwm.channel, pwm.pin->pin, TCCR2A);
    write_result(buffer, size);
}

void test_close_timer0_a()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_A);
    rfs_pwm_close(&pwm);
    uint8_t size = sprintf(buffer, "7:%hhx,%hhx\n", TCCR0A, TCCR0B);
    write_result(buffer, size);
}

void test_close_timer0_b()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_B);
    rfs_pwm_close(&pwm);
    uint8_t size = sprintf(buffer, "8:%hhx,%hhx\n", TCCR0A, TCCR0B);
    write_result(buffer, size);
}

void test_close_timer1_a()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER1, RFS_PWM_CHANNEL_A);
    rfs_pwm_close(&pwm);
    uint8_t size = sprintf(buffer, "9:%hhx,%hhx\n", TCCR1A, TCCR1B);
    write_result(buffer, size);
}

void test_close_timer1_b()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER1, RFS_PWM_CHANNEL_B);
    rfs_pwm_close(&pwm);
    uint8_t size = sprintf(buffer, "10:%hhx,%hhx\n", TCCR1A, TCCR1B);
    write_result(buffer, size);
}

void test_close_timer2_a()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2, RFS_PWM_CHANNEL_A);
    rfs_pwm_close(&pwm);
    uint8_t size = sprintf(buffer, "11:%hhx,%hhx\n", TCCR2A, TCCR2B);
    write_result(buffer, size);
}

void test_close_timer2_b()
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2, RFS_PWM_CHANNEL_B);
    rfs_pwm_close(&pwm);
    uint8_t size = sprintf(buffer, "12:%hhx,%hhx\n", TCCR2A, TCCR2B);
    write_result(buffer, size);
}

/*

void test_set_frequency_timer0(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0);
    rfs_pwm_set_frequency(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx\n", test_id, TCCR0A, TCCR0B);
    write_result(buffer, size);
}

void test_set_frequency_timer2(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2);
    rfs_pwm_set_frequency(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx\n", test_id, TCCR2A, TCCR2B);
    write_result(buffer, size);
}

void test_set_frequency_exact_timer0(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0);
    rfs_pwm_set_frequency_exact(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx,%hhx\n", test_id, TCCR0A, TCCR0B, OCR0A);
    write_result(buffer, size);
}

void test_set_frequency_exact_timer2(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2);
    rfs_pwm_set_frequency_exact(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx,%hhx\n", test_id, TCCR2A, TCCR2B, OCR2A);
    write_result(buffer, size);
}

void test_set_duty_cycle_channel_a_timer0(uint8_t test_id, uint8_t duty_cycle)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0);
    rfs_pwm_set_duty_cycle_channel_a_8(&pwm, duty_cycle);
    uint8_t size = sprintf(buffer, "%hhu:%hhx\n", test_id, OCR0A);
    write_result(buffer, size);
}

void test_set_duty_cycle_channel_b_timer0(uint8_t test_id, uint8_t duty_cycle)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0);
    rfs_pwm_set_duty_cycle_channel_b_8(&pwm, duty_cycle);
    uint8_t size = sprintf(buffer, "%hhu:%hhx\n", test_id, OCR0B);
    write_result(buffer, size);
}

void test_set_duty_cycle_channel_a_timer2(uint8_t test_id, uint8_t duty_cycle)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2);
    rfs_pwm_set_duty_cycle_channel_a_8(&pwm, duty_cycle);
    uint8_t size = sprintf(buffer, "%hhu:%hhx\n", test_id, OCR2A);
    write_result(buffer, size);
}

void test_set_duty_cycle_channel_b_timer2(uint8_t test_id, uint8_t duty_cycle)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2);
    rfs_pwm_set_duty_cycle_channel_b_8(&pwm, duty_cycle);
    uint8_t size = sprintf(buffer, "%hhu:%hhx\n", test_id, OCR2B);
    write_result(buffer, size);
}

*/

int main()
{
    init_usart();

    test_init_timer0_a();
    test_init_timer0_b();
    test_init_timer1_a();
    test_init_timer1_b();
    test_init_timer2_a();
    test_init_timer2_b();
    test_close_timer0_a();
    test_close_timer0_b();
    test_close_timer1_a();
    test_close_timer1_b();
    test_close_timer2_a();
    test_close_timer2_b();
    /*test_init_timer2();
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
    test_set_frequency_exact_timer0(53, 70000);
    test_set_frequency_exact_timer0(54, 62501);
    test_set_frequency_exact_timer0(55, 62500);
    test_set_frequency_exact_timer0(56, 31251);
    test_set_frequency_exact_timer0(57, 31250);
    test_set_frequency_exact_timer0(58, 7813);
    test_set_frequency_exact_timer0(59, 7812);
    test_set_frequency_exact_timer0(60, 3907);
    test_set_frequency_exact_timer0(61, 3906);
    test_set_frequency_exact_timer0(62, 977);
    test_set_frequency_exact_timer0(63, 976);
    test_set_frequency_exact_timer0(64, 489);
    test_set_frequency_exact_timer0(65, 488);
    test_set_frequency_exact_timer0(66, 245);
    test_set_frequency_exact_timer0(67, 244);
    test_set_frequency_exact_timer0(68, 123);
    test_set_frequency_exact_timer0(69, 122);
    test_set_frequency_exact_timer0(70, 62);
    test_set_frequency_exact_timer0(71, 61);
    test_set_frequency_exact_timer0(72, 31);
    test_set_frequency_exact_timer0(73, 30);
    test_set_frequency_exact_timer2(74, 70000);
    test_set_frequency_exact_timer2(75, 62501);
    test_set_frequency_exact_timer2(76, 62500);
    test_set_frequency_exact_timer2(77, 31251);
    test_set_frequency_exact_timer2(78, 31250);
    test_set_frequency_exact_timer2(79, 7813);
    test_set_frequency_exact_timer2(80, 7812);
    test_set_frequency_exact_timer2(81, 3907);
    test_set_frequency_exact_timer2(82, 3906);
    test_set_frequency_exact_timer2(83, 1954);
    test_set_frequency_exact_timer2(84, 1953);
    test_set_frequency_exact_timer2(85, 977);
    test_set_frequency_exact_timer2(86, 976);
    test_set_frequency_exact_timer2(87, 489);
    test_set_frequency_exact_timer2(88, 488);
    test_set_frequency_exact_timer2(89, 245);
    test_set_frequency_exact_timer2(90, 244);
    test_set_frequency_exact_timer2(91, 123);
    test_set_frequency_exact_timer2(92, 122);
    test_set_frequency_exact_timer2(93, 62);
    test_set_frequency_exact_timer2(94, 61);
    test_set_frequency_exact_timer2(95, 31);
    test_set_frequency_exact_timer2(96, 30);
    test_set_duty_cycle_channel_a_timer0(97, 0);
    test_set_duty_cycle_channel_a_timer0(98, 128);
    test_set_duty_cycle_channel_a_timer0(99, 255);
    test_set_duty_cycle_channel_b_timer0(100, 0);
    test_set_duty_cycle_channel_b_timer0(101, 128);
    test_set_duty_cycle_channel_b_timer0(102, 255);
    test_set_duty_cycle_channel_a_timer2(103, 0);
    test_set_duty_cycle_channel_a_timer2(104, 128);
    test_set_duty_cycle_channel_a_timer2(105, 255);
    test_set_duty_cycle_channel_b_timer2(106, 0);
    test_set_duty_cycle_channel_b_timer2(107, 128);
    test_set_duty_cycle_channel_b_timer2(108, 255);
    test_close_timer0();
    test_close_timer2();

    test_enable_channel_A_timer1();
    test_enable_channel_B_timer1();
    test_disable_channel_A_timer1();
    test_disable_channel_B_timer1();*/

    // Loop forever
    while (1);
}