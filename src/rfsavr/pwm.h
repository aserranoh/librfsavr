/*
pwm.h - Generate PWM signals using the AVR microcontroller.

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

#ifndef RFS_PWM_H
#define RFS_PWM_H

#include "rfsavr/timers.h"

/**
 * @brief Struct that contains the information to control an 8-bit PWM signal
 */
struct rfs_pwm8_t {
    struct rfs_timer8_t timer;
};

/**
 * @brief Initialize the 8-bit PWM
 * 
 * @param pwm The structure that contains the PWM information
 * @param timer Which timer to use to control the PWM signal
 */
inline void rfs_pwm8_init(struct rfs_pwm8_t *pwm, enum rfs_timer8_enum timer)
{
    rfs_timer8_init(&(pwm->timer), timer);
}

/**
 * @brief Disable PWM output on channel A
 * 
 * @param pwm The structure that contains the PWM information
 */
inline void rfs_pwm8_disable_channel_A(struct rfs_pwm8_t *pwm)
{
    rfs_timer8_set_compare_match_output_mode_A(&pwm->timer, RFS_TIMER_COMA_NORMAL);
}

/**
 * @brief Disable PWM output on channel B
 * 
 * @param pwm The structure that contains the PWM information
 */
inline void rfs_pwm8_disable_channel_B(struct rfs_pwm8_t *pwm)
{
    rfs_timer8_set_compare_match_output_mode_B(&pwm->timer, RFS_TIMER_COMB_NORMAL);
}

/**
 * @brief Enable PWM output on channel A
 * 
 * @param pwm The structure that contains the PWM information
 */
void rfs_pwm8_enable_channel_A(struct rfs_pwm8_t *pwm);

/**
 * @brief Enable PWM output on channel B
 * 
 * @param pwm The structure that contains the PWM information
 */
void rfs_pwm8_enable_channel_B(struct rfs_pwm8_t *pwm);

/**
 * @brief Set the PWM signal frequency
 * 
 * @param pwm The structure that contains the PWM information
 * @param frequency The PWM signal frequency
 * @param cpu_frequency The CPU's clock frequency
 */
void rfs_pwm8_set_frequency(struct rfs_pwm8_t *pwm, uint32_t frequency, uint32_t cpu_frequency);

/*void rfs_pwm_set_duty_cycle_8(struct rfs_pwm_t *pwm, uint8_t duty_cycle);
void rfs_pwm_set_duty_cycle_16(struct rfs_pwm_t *pwm, uint16_t duty_cycle);
void rfs_pwm_close(struct rfs_pwm_t *pwm);*/

#endif