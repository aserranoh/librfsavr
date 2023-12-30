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

void rfs_pwm8_init(struct rfs_pwm8_t *pwm, enum rfs_timer8_enum timer)
{
    rfs_timer8_init(&(pwm->timer), timer);
    switch (timer) {
    case RFS_TIMER0:
        pwm->divisor_table = RFS_TIMER0_TABLE;
        pwm->divisor_table_size = RFS_TIMER0_TABLE_SIZE;
        break;
    case RFS_TIMER2:
        pwm->divisor_table = RFS_TIMER2_TABLE;
        pwm->divisor_table_size = RFS_TIMER2_TABLE_SIZE;
        break;
    }
}

static int8_t rfs_pwm8_set_clock_divisor_and_mode(struct rfs_pwm8_t *pwm, uint32_t target_frequency, uint32_t cpu_clock_frequency)
{
    uint32_t divisor = (cpu_clock_frequency >> 8) / target_frequency;
    enum rfs_timer8_mode mode = RFS_TIMER8_MODE_FAST_PWM;

    int8_t divisor_index = 0;
    int8_t i = 0;
    while (i < pwm->divisor_table_size && pwm->divisor_table[i] <= divisor) {
        divisor_index = i++;
    }
    if ((pwm->divisor_table[divisor_index] << 1) <= divisor) {
        mode = RFS_TIMER8_MODE_PWM_PHASE_CORRECT;
    }

    rfs_timer8_set_mode(&pwm->timer, mode);
    rfs_timer8_set_clock(&pwm->timer, divisor_index + 1);

    return divisor_index;
}

void rfs_pwm8_set_frequency(struct rfs_pwm8_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    rfs_pwm8_set_clock_divisor_and_mode(pwm, frequency, cpu_frequency);
}

void rfs_pwm8_set_frequency_exact(struct rfs_pwm8_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    uint8_t ocra;
    int8_t divisor_index = rfs_pwm8_set_clock_divisor_and_mode(pwm, frequency, cpu_frequency);
    enum rfs_timer8_mode mode = rfs_timer8_get_mode(&pwm->timer);

    // Extreme case, the requested frequency is greater than the maximum frequency
    if (frequency > (cpu_frequency >> 8)) {
        mode = RFS_TIMER8_MODE_FAST_PWM_OCRA;
        ocra = cpu_frequency / (frequency * pwm->divisor_table[divisor_index]);

    // If the mode set by rfs_pwm8_set_clock_divisor_and_mode is FAST_PWM, then switch
    // to PWM_PHASE_CORRECT and compute OCRA with double the divisor
    } else if (mode == RFS_TIMER8_MODE_FAST_PWM) {
        mode = RFS_TIMER8_MODE_PWM_PHASE_CORRECT_OCRA;
        ocra = cpu_frequency / (frequency * (pwm->divisor_table[divisor_index] << 1));

    // Finaly, if the mode set by rfs_pwm8_set_clock_divisor_and_mode is PHASE_CORRECT and
    // there's still a greater divisor, switch to FAST_PWM mode and use the next divisor
    // in the table (the clock source must be switched as well)
    } else if (divisor_index + 1 < pwm->divisor_table_size) {
        mode = RFS_TIMER8_MODE_FAST_PWM_OCRA;
        ocra = cpu_frequency / (frequency * pwm->divisor_table[divisor_index + 1]);
        rfs_timer8_set_clock(&pwm->timer, divisor_index + 2);
    }

    rfs_timer8_set_mode(&pwm->timer, mode);
    rfs_timer8_set_ocra(&pwm->timer, ocra);
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