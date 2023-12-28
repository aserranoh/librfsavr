/*
pwm.c - Generate PWM signals using the AVR microcontroller.

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

static uint16_t RFS_TIMER0_TABLE[] = {1, 8, 64, 256, 1024};
static int8_t RFS_TIMER0_TABLE_SIZE = 5;
static uint16_t RFS_TIMER2_TABLE[] = {1, 8, 32, 64, 128, 256, 1024};
static int8_t RFS_TIMER2_TABLE_SIZE = 7;

void rfs_pwm8_enable_channel_A(struct rfs_pwm8_t *pwm)
{
    rfs_timer8_set_compare_match_output_mode_A(&pwm->timer, RFS_TIMER_COMA_NONINVERT);
    if (pwm->timer.cra == &TCCR0A) {
        DDRD |= _BV(6);
    } else {
        DDRB |= _BV(3);
    }
}

void rfs_pwm8_enable_channel_B(struct rfs_pwm8_t *pwm)
{
    rfs_timer8_set_compare_match_output_mode_B(&pwm->timer, RFS_TIMER_COMB_NONINVERT);
    if (pwm->timer.cra == &TCCR0A) {
        DDRD |= _BV(5);
    } else {
        DDRD |= _BV(3);
    }
}

static void rfs_pwm8_set_clock_divisor_and_mode(
    struct rfs_pwm8_t *pwm,
    uint32_t target_frequency,
    uint32_t cpu_clock_frequency,
    uint16_t *table,
    int8_t table_size)
{
    uint32_t divisor = (cpu_clock_frequency >> 8) / target_frequency;
    enum rfs_timer8_mode mode = RFS_TIMER8_MODE_PWM_PHASE_CORRECT;

    int8_t divisor_index = -1;
    do {
        divisor_index++;
        if (table[divisor_index] > divisor) {
            mode = RFS_TIMER8_MODE_FAST_PWM;
            break;
        } else if ((table[divisor_index] << 1) > divisor) {
            break;
        }
    } while (divisor_index < table_size - 1);

    rfs_timer8_set_mode(&pwm->timer, mode);
    rfs_timer8_set_clock(&pwm->timer, divisor_index + 1);
}

void rfs_pwm8_set_frequency(struct rfs_pwm8_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    uint16_t *table;
    int8_t table_size;

    if (pwm->timer.cra == &TCCR0A) {
        table = RFS_TIMER0_TABLE;
        table_size = RFS_TIMER0_TABLE_SIZE;
    } else {
        table = RFS_TIMER2_TABLE;
        table_size = RFS_TIMER2_TABLE_SIZE;
    }
    rfs_pwm8_set_clock_divisor_and_mode(pwm, frequency, cpu_frequency, table, table_size);
}

/*void rfs_pwm_set_duty_cycle_8(struct rfs_pwm_t *pwm, uint8_t duty_cycle)
{

}

void rfs_pwm_set_duty_cycle_16(struct rfs_pwm_t *pwm, uint16_t duty_cycle)
{

}

void rfs_pwm_close(struct rfs_pwm_t *pwm)
{

}*/