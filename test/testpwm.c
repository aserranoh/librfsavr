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

void test_set_frequency_hint_timer0(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_A);
    rfs_pwm_set_frequency_hint(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx\n", test_id, TCCR0A, TCCR0B);
    write_result(buffer, size);
}

void test_set_frequency_hint_timer2(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2, RFS_PWM_CHANNEL_B);
    rfs_pwm_set_frequency_hint(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx\n", test_id, TCCR2A, TCCR2B);
    write_result(buffer, size);
}

void test_set_frequency_hint_timer1(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER1, RFS_PWM_CHANNEL_A);
    rfs_pwm_set_frequency_hint(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx\n", test_id, TCCR1A, TCCR1B);
    write_result(buffer, size);
}

void test_set_frequency_timer0(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_B);
    rfs_pwm_set_frequency(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx,%hhx\n", test_id, TCCR0A, TCCR0B, OCR0A);
    write_result(buffer, size);
}

void test_set_frequency_timer2(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2, RFS_PWM_CHANNEL_A);
    rfs_pwm_set_frequency(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx,%hhx\n", test_id, TCCR2A, TCCR2B, OCR2A);
    write_result(buffer, size);
}

void test_set_frequency_timer1(uint8_t test_id, uint32_t frequency)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER1, RFS_PWM_CHANNEL_A);
    rfs_pwm_set_frequency(&pwm, frequency, F_CPU);
    uint8_t size = sprintf(buffer, "%hhu:%hhx,%hhx,%hx\n", test_id, TCCR1A, TCCR1B, OCR1A);
    write_result(buffer, size);
}

void test_set_duty_cycle_channel_a_timer0(uint8_t test_id, uint8_t duty_cycle)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_A);
    rfs_pwm_set_duty_cycle_8(&pwm, duty_cycle);
    uint8_t size = sprintf(buffer, "%hhu:%hhx\n", test_id, OCR0A);
    write_result(buffer, size);
}

void test_set_duty_cycle_channel_b_timer0(uint8_t test_id, uint8_t duty_cycle)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER0, RFS_PWM_CHANNEL_B);
    rfs_pwm_set_duty_cycle_8(&pwm, duty_cycle);
    uint8_t size = sprintf(buffer, "%hhu:%hhx\n", test_id, OCR0B);
    write_result(buffer, size);
}

void test_set_duty_cycle_channel_a_timer2(uint8_t test_id, uint8_t duty_cycle)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2, RFS_PWM_CHANNEL_A);
    rfs_pwm_set_duty_cycle_8(&pwm, duty_cycle);
    uint8_t size = sprintf(buffer, "%hhu:%hhx\n", test_id, OCR2A);
    write_result(buffer, size);
}

void test_set_duty_cycle_channel_b_timer2(uint8_t test_id, uint8_t duty_cycle)
{
    struct rfs_pwm_t pwm;
    rfs_pwm_init(&pwm, RFS_TIMER2, RFS_PWM_CHANNEL_B);
    rfs_pwm_set_duty_cycle_8(&pwm, duty_cycle);
    uint8_t size = sprintf(buffer, "%hhu:%hhx\n", test_id, OCR2B);
    write_result(buffer, size);
}

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
    test_set_frequency_hint_timer0(13, 62501);
    test_set_frequency_hint_timer0(14, 62500);
    test_set_frequency_hint_timer0(15, 31251);
    test_set_frequency_hint_timer0(16, 31250);
    test_set_frequency_hint_timer0(17, 7813);
    test_set_frequency_hint_timer0(18, 7812);
    test_set_frequency_hint_timer0(19, 3907);
    test_set_frequency_hint_timer0(20, 3906);
    test_set_frequency_hint_timer0(21, 977);
    test_set_frequency_hint_timer0(22, 976);
    test_set_frequency_hint_timer0(23, 489);
    test_set_frequency_hint_timer0(24, 488);
    test_set_frequency_hint_timer0(25, 245);
    test_set_frequency_hint_timer0(26, 244);
    test_set_frequency_hint_timer0(27, 123);
    test_set_frequency_hint_timer0(28, 122);
    test_set_frequency_hint_timer0(29, 62);
    test_set_frequency_hint_timer0(30, 61);
    test_set_frequency_hint_timer0(31, 31);
    test_set_frequency_hint_timer0(32, 30);
    test_set_frequency_hint_timer2(33, 62501);
    test_set_frequency_hint_timer2(34, 62500);
    test_set_frequency_hint_timer2(35, 31251);
    test_set_frequency_hint_timer2(36, 31250);
    test_set_frequency_hint_timer2(37, 7813);
    test_set_frequency_hint_timer2(38, 7812);
    test_set_frequency_hint_timer2(39, 3907);
    test_set_frequency_hint_timer2(40, 3906);
    test_set_frequency_hint_timer2(41, 1954);
    test_set_frequency_hint_timer2(42, 1953);
    test_set_frequency_hint_timer2(43, 977);
    test_set_frequency_hint_timer2(44, 976);
    test_set_frequency_hint_timer2(45, 489);
    test_set_frequency_hint_timer2(46, 488);
    test_set_frequency_hint_timer2(47, 245);
    test_set_frequency_hint_timer2(48, 244);
    test_set_frequency_hint_timer2(49, 123);
    test_set_frequency_hint_timer2(50, 122);
    test_set_frequency_hint_timer2(51, 62);
    test_set_frequency_hint_timer2(52, 61);
    test_set_frequency_hint_timer2(53, 31);
    test_set_frequency_hint_timer2(54, 30);
    test_set_frequency_hint_timer1(55, 62501);
    test_set_frequency_hint_timer1(56, 62500);
    test_set_frequency_hint_timer1(57, 31251);
    test_set_frequency_hint_timer1(58, 31250);
    test_set_frequency_hint_timer1(59, 15626);
    test_set_frequency_hint_timer1(60, 15625);
    test_set_frequency_hint_timer1(61, 7813);
    test_set_frequency_hint_timer1(62, 7812);
    test_set_frequency_hint_timer1(63, 1954);
    test_set_frequency_hint_timer1(64, 1953);
    test_set_frequency_hint_timer1(65, 977);
    test_set_frequency_hint_timer1(66, 976);
    test_set_frequency_hint_timer1(67, 245);
    test_set_frequency_hint_timer1(68, 244);
    test_set_frequency_hint_timer1(69, 123);
    test_set_frequency_hint_timer1(70, 122);
    test_set_frequency_hint_timer1(71, 62);
    test_set_frequency_hint_timer1(72, 61);
    test_set_frequency_hint_timer1(73, 31);
    test_set_frequency_hint_timer1(74, 30);
    test_set_frequency_hint_timer1(75, 16);
    test_set_frequency_hint_timer1(76, 15);
    test_set_frequency_hint_timer1(77, 8);
    test_set_frequency_hint_timer1(78, 7);
    test_set_frequency_timer0(79, 70000);
    test_set_frequency_timer0(80, 62501);
    test_set_frequency_timer0(81, 62500);
    test_set_frequency_timer0(82, 31251);
    test_set_frequency_timer0(83, 31250);
    test_set_frequency_timer0(84, 7813);
    test_set_frequency_timer0(85, 7812);
    test_set_frequency_timer0(86, 3907);
    test_set_frequency_timer0(87, 3906);
    test_set_frequency_timer0(88, 977);
    test_set_frequency_timer0(89, 976);
    test_set_frequency_timer0(90, 489);
    test_set_frequency_timer0(91, 488);
    test_set_frequency_timer0(92, 245);
    test_set_frequency_timer0(93, 244);
    test_set_frequency_timer0(94, 123);
    test_set_frequency_timer0(95, 122);
    test_set_frequency_timer0(96, 62);
    test_set_frequency_timer0(97, 61);
    test_set_frequency_timer0(98, 31);
    test_set_frequency_timer0(99, 30);
    test_set_frequency_timer2(100, 70000);
    test_set_frequency_timer2(101, 62501);
    test_set_frequency_timer2(102, 62500);
    test_set_frequency_timer2(103, 31251);
    test_set_frequency_timer2(104, 31250);
    test_set_frequency_timer2(105, 7813);
    test_set_frequency_timer2(106, 7812);
    test_set_frequency_timer2(107, 3907);
    test_set_frequency_timer2(108, 3906);
    test_set_frequency_timer2(109, 1954);
    test_set_frequency_timer2(110, 1953);
    test_set_frequency_timer2(111, 977);
    test_set_frequency_timer2(112, 976);
    test_set_frequency_timer2(113, 489);
    test_set_frequency_timer2(114, 488);
    test_set_frequency_timer2(115, 245);
    test_set_frequency_timer2(116, 244);
    test_set_frequency_timer2(117, 123);
    test_set_frequency_timer2(118, 122);
    test_set_frequency_timer2(119, 62);
    test_set_frequency_timer2(120, 61);
    test_set_frequency_timer2(121, 31);
    test_set_frequency_timer2(122, 30);
    test_set_frequency_timer1(123, 300);
    test_set_frequency_timer1(124, 245);
    test_set_frequency_timer1(125, 244);
    test_set_frequency_timer1(126, 123);
    test_set_frequency_timer1(127, 122);
    test_set_frequency_timer1(128, 31);
    test_set_frequency_timer1(129, 30);
    test_set_frequency_timer1(130, 16);
    test_set_frequency_timer1(131, 15);
    test_set_frequency_timer1(132, 4);
    test_set_frequency_timer1(133, 3);
    test_set_frequency_timer1(134, 2);
    test_set_frequency_timer1(135, 1);
    test_set_duty_cycle_channel_a_timer0(136, 0);
    test_set_duty_cycle_channel_a_timer0(137, 128);
    test_set_duty_cycle_channel_a_timer0(138, 255);
    test_set_duty_cycle_channel_b_timer0(139, 0);
    test_set_duty_cycle_channel_b_timer0(140, 128);
    test_set_duty_cycle_channel_b_timer0(141, 255);
    test_set_duty_cycle_channel_a_timer2(142, 0);
    test_set_duty_cycle_channel_a_timer2(143, 128);
    test_set_duty_cycle_channel_a_timer2(144, 255);
    test_set_duty_cycle_channel_b_timer2(145, 0);
    test_set_duty_cycle_channel_b_timer2(146, 128);
    test_set_duty_cycle_channel_b_timer2(147, 255);

    // Loop forever
    while (1);
}