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

enum rfs_pwm_mode {
    RFS_PWM_PHASE_CORRECT,
    RFS_PWM_FAST
};

struct rfs_pwm_divisor_mode_t {
    enum rfs_pwm_mode mode;
    int8_t divisor;
};

static void rfs_pwm_set_frequency_8(const struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency);
static void rfs_pwm_set_frequency_16(const struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency);
static void rfs_pwm_set_frequency_hint_8(const struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency);
static void rfs_pwm_set_frequency_hint_16(const struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency);

static const void (*RFS_PWM_SET_FREQUENCY_FUNCTION_TABLE[3])(const struct rfs_pwm_t *, uint32_t, uint32_t) = {
    rfs_pwm_set_frequency_8,
    rfs_pwm_set_frequency_16,
    rfs_pwm_set_frequency_8
};

static const void (*RFS_PWM_SET_FREQUENCY_FUNCTION_TABLE_HINT[3])(const struct rfs_pwm_t *, uint32_t, uint32_t) = {
    rfs_pwm_set_frequency_hint_8,
    rfs_pwm_set_frequency_hint_16,
    rfs_pwm_set_frequency_hint_8
};

void rfs_pwm_init(struct rfs_pwm_t *pwm, enum rfs_timer_enum timer, enum rfs_pwm_channel channel)
{
    rfs_timer_init(&(pwm->timer), timer);
    pwm->channel = channel;
    pwm->pin = &rfs_timer_compare_output(timer, channel);
    rfs_pin_set_output(pwm->pin);
    pwm->divisor_table = &rfs_timer_divisor_table(timer);
    pwm->set_frequency = RFS_PWM_SET_FREQUENCY_FUNCTION_TABLE[timer];
    pwm->set_frequency_hint = RFS_PWM_SET_FREQUENCY_FUNCTION_TABLE_HINT[timer];
    if (channel == RFS_PWM_CHANNEL_A) {
        rfs_timer_set_compare_match_output_mode_a(&pwm->timer, RFS_TIMER_COMA_NONINVERT);
        if (timer == RFS_TIMER0 || timer == RFS_TIMER2) {
            pwm->ocr8 = pwm->timer.ocra8;
        } else {
            pwm->ocr16 = pwm->timer.ocra16;
        }
    } else {
        rfs_timer_set_compare_match_output_mode_b(&pwm->timer, RFS_TIMER_COMB_NONINVERT);
        if (timer == RFS_TIMER0 || timer == RFS_TIMER2) {
            pwm->ocr8 = pwm->timer.ocrb8;
        } else {
            pwm->ocr16 = pwm->timer.ocrb16;
        }
    }
}

void rfs_pwm_close(const struct rfs_pwm_t *pwm)
{
    if (pwm->channel == RFS_PWM_CHANNEL_A) {
        rfs_timer_set_compare_match_output_mode_a(&pwm->timer, RFS_TIMER_COMA_NORMAL);
    } else {
        rfs_timer_set_compare_match_output_mode_b(&pwm->timer, RFS_TIMER_COMB_NORMAL);
    }
    rfs_timer_set_mode_8(&pwm->timer, RFS_TIMER8_MODE_NORMAL);
    rfs_timer_set_clock(&pwm->timer, RFS_TIMER_CLOCK_NONE);
}

static void rfs_pwm_set_clock_divisor_and_mode(const struct rfs_pwm_t *pwm, uint32_t target_frequency, uint32_t max_frequency,
    struct rfs_pwm_divisor_mode_t *divisor_mode)
{
    const uint16_t divisor = max_frequency / target_frequency;
    enum rfs_pwm_mode mode = RFS_PWM_PHASE_CORRECT;
    int8_t divisor_index = -1;

    do {
        divisor_index++;
        const uint16_t table_divisor = rfs_list_get(*pwm->divisor_table, divisor_index);
        if (divisor < table_divisor) {
            mode = RFS_PWM_FAST;
            break;
        } else if (divisor < (table_divisor << 1)) {
            break;
        }
    } while (divisor_index < rfs_list_size(*pwm->divisor_table) - 1);

    divisor_mode->mode = mode;
    divisor_mode->divisor = divisor_index;
}

static void rfs_pwm_set_frequency_8(const struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    const uint32_t max_frequency = cpu_frequency >> 8;
    struct rfs_pwm_divisor_mode_t divisor_mode;

    rfs_pwm_set_clock_divisor_and_mode(pwm, frequency, max_frequency, &divisor_mode);

    const uint32_t divisor = (divisor_mode.mode == RFS_PWM_FAST)
        ? (frequency * rfs_list_get(*pwm->divisor_table, divisor_mode.divisor))
        : (frequency * (rfs_list_get(*pwm->divisor_table, divisor_mode.divisor) << 1));
    
    rfs_timer_set_mode_8(&pwm->timer, (divisor_mode.mode << 1) | 0b1001);
    rfs_timer_set_clock(&pwm->timer, divisor_mode.divisor + 1);
    rfs_timer_set_ocra_8(&pwm->timer, (divisor > max_frequency) ? (cpu_frequency/divisor) : 0xff);
}

static void rfs_pwm_set_frequency_16(const struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    const uint32_t max_frequency = cpu_frequency >> 16;
    struct rfs_pwm_divisor_mode_t divisor_mode;

    rfs_pwm_set_clock_divisor_and_mode(pwm, frequency, max_frequency, &divisor_mode);

    const uint32_t divisor = (divisor_mode.mode == RFS_PWM_FAST)
        ? (frequency * rfs_list_get(*pwm->divisor_table, divisor_mode.divisor))
        : (frequency * (rfs_list_get(*pwm->divisor_table, divisor_mode.divisor) << 1));
    
    rfs_timer_set_mode_16(&pwm->timer, (divisor_mode.mode << 3) | 0b10010);
    rfs_timer_set_clock(&pwm->timer, divisor_mode.divisor + 1);
    rfs_timer_set_icr(&pwm->timer, (divisor > max_frequency) ? (cpu_frequency/divisor) : 0xffff);
}

static void rfs_pwm_set_frequency_hint_8(const struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    const uint32_t max_frequency = cpu_frequency >> 8;
    struct rfs_pwm_divisor_mode_t divisor_mode;
    rfs_pwm_set_clock_divisor_and_mode(pwm, frequency, max_frequency, &divisor_mode);
    rfs_timer_set_mode_8(&pwm->timer, (divisor_mode.mode << 1) + 1);
    rfs_timer_set_clock(&pwm->timer, divisor_mode.divisor + 1);
}

static void rfs_pwm_set_frequency_hint_16(const struct rfs_pwm_t *pwm, uint32_t frequency, uint32_t cpu_frequency)
{
    uint32_t max_frequency = cpu_frequency >> 10;
    int8_t bits = 3;
    struct rfs_pwm_divisor_mode_t divisor_mode;

    if (frequency > max_frequency) {
        max_frequency = cpu_frequency >> 9;
        bits = 2;
        if (frequency > max_frequency) {
            max_frequency = cpu_frequency >> 8;
            bits = 1;
        }
    }
    rfs_pwm_set_clock_divisor_and_mode(pwm, frequency, max_frequency, &divisor_mode);
    rfs_timer_set_mode_16(&pwm->timer, (divisor_mode.mode << 3) + bits);
    rfs_timer_set_clock(&pwm->timer, divisor_mode.divisor + 1);
}
