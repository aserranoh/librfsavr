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

void rfs_pwm_init(struct rfs_pwm_t *pwm, enum rfs_timer_enum timer, enum rfs_pwm_channel channel)
{
    rfs_timer_init(&(pwm->timer), timer);
    pwm->channel = channel;
    pwm->pin = &rfs_timer_compare_output(timer, channel);
    rfs_pin_set_output(pwm->pin);
    pwm->divisor_table = &rfs_timer_divisor_table(timer);
    if (channel == RFS_PWM_CHANNEL_A) {
        rfs_timer_set_compare_match_output_mode_a(&pwm->timer, RFS_TIMER_COMA_NONINVERT);
        pwm->ocr = pwm->timer.ocra;
    } else {
        rfs_timer_set_compare_match_output_mode_b(&pwm->timer, RFS_TIMER_COMB_NONINVERT);
        pwm->ocr = pwm->timer.ocrb;
    }
}

void rfs_pwm_close(struct rfs_pwm_t *pwm)
{
    if (pwm->channel == RFS_PWM_CHANNEL_A) {
        rfs_timer_set_compare_match_output_mode_a(&pwm->timer, RFS_TIMER_COMA_NORMAL);
    } else {
        rfs_timer_set_compare_match_output_mode_b(&pwm->timer, RFS_TIMER_COMB_NORMAL);
    }
    rfs_timer_set_mode_8(&pwm->timer, RFS_TIMER8_MODE_NORMAL);
    rfs_timer_set_clock(&pwm->timer, RFS_TIMER_CLOCK_NONE);
}

static int8_t rfs_pwm_set_clock_divisor_and_mode(struct rfs_pwm_t *pwm, uint32_t target_frequency, uint32_t cpu_clock_frequency)
{
    uint32_t divisor = (cpu_clock_frequency >> 8) / target_frequency;
    enum rfs_timer_mode_8 mode = RFS_TIMER8_MODE_FAST_PWM;

    int8_t divisor_index = 0;
    int8_t i = 0;
    while (i < rfs_list_size(*pwm->divisor_table) && rfs_list_get(*pwm->divisor_table, i) <= divisor) {
        divisor_index = i++;
    }
    if ((rfs_list_get(*pwm->divisor_table, divisor_index) << 1) <= divisor) {
        mode = RFS_TIMER8_MODE_PWM_PHASE_CORRECT;
    }

    rfs_timer_set_mode_8(&pwm->timer, mode);
    rfs_timer_set_clock(&pwm->timer, divisor_index + 1);

    return divisor_index;
}

void rfs_pwm_set_frequency(struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    rfs_pwm_set_clock_divisor_and_mode(pwm, frequency, cpu_frequency);
}

void rfs_pwm_set_frequency_exact(struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    uint8_t ocra;
    int8_t divisor_index = rfs_pwm_set_clock_divisor_and_mode(pwm, frequency, cpu_frequency);
    enum rfs_timer_mode_8 mode = rfs_timer_get_mode(&pwm->timer);

    // Extreme case, the requested frequency is greater than the maximum frequency
    if (frequency > (cpu_frequency >> 8)) {
        mode = RFS_TIMER8_MODE_FAST_PWM_OCRA;
        ocra = cpu_frequency / (frequency * rfs_list_get(*pwm->divisor_table, divisor_index));

    // If the mode set by rfs_pwm8_set_clock_divisor_and_mode is FAST_PWM, then switch
    // to PWM_PHASE_CORRECT and compute OCRA with double the divisor
    } else if (mode == RFS_TIMER8_MODE_FAST_PWM) {
        mode = RFS_TIMER8_MODE_PWM_PHASE_CORRECT_OCRA;
        ocra = cpu_frequency / (frequency * (rfs_list_get(*pwm->divisor_table, divisor_index) << 1));

    // Finaly, if the mode set by rfs_pwm8_set_clock_divisor_and_mode is PHASE_CORRECT and
    // there's still a greater divisor, switch to FAST_PWM mode and use the next divisor
    // in the table (the clock source must be switched as well)
    } else if (divisor_index + 1 < rfs_list_size(*pwm->divisor_table)) {
        mode = RFS_TIMER8_MODE_FAST_PWM_OCRA;
        ocra = cpu_frequency / (frequency * rfs_list_get(*pwm->divisor_table, divisor_index + 1));
        rfs_timer_set_clock(&pwm->timer, divisor_index + 2);

    // In the last case, we are under the minimum frequency
    } else {
        mode = RFS_TIMER8_MODE_PWM_PHASE_CORRECT_OCRA;
        ocra = 255;
    }

    rfs_timer_set_mode_8(&pwm->timer, mode);
    rfs_timer_set_ocra_8(&pwm->timer, ocra);
}
