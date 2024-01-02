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

#include "rfsavr/io.h"
#include "rfsavr/timers.h"

/**
 * @brief Struct that contains the information to control an 8-bit PWM signal
 */
struct rfs_pwm8_t {
    struct rfs_timer8_t timer;
    struct rfs_pin_t output_a;
    struct rfs_pin_t output_b;
    uint16_t *divisor_table;
    int8_t divisor_table_size;
};

/**
 * @brief Struct that contains the information to control a 16-bit PWM signal
 */
struct rfs_pwm16_t {
    struct rfs_timer16_t timer;
    struct rfs_pin_t output_a;
    struct rfs_pin_t output_b;
    uint16_t *divisor_table;
    int8_t divisor_table_size;
};

/**
 * @brief Initialize the 8-bit PWM
 * 
 * @param pwm The structure that contains the PWM information
 * @param timer Which timer to use to control the PWM signal
 */
void rfs_pwm8_init(struct rfs_pwm8_t *pwm, enum rfs_timer8_enum timer);

/**
 * @brief Shutdown the 8-bit PWM
 * 
 * @param pwm The structure that contains the PWM information
 */
void rfs_pwm8_close(struct rfs_pwm8_t *pwm);

/**
 * @brief Disable PWM output on channel A
 * 
 * @param pwm The structure that contains the PWM information
 */
inline void rfs_pwm8_disable_channel_a(struct rfs_pwm8_t *pwm)
{
    rfs_timer8_set_compare_match_output_mode_a(&pwm->timer, RFS_TIMER_COMA_NORMAL);
}

/**
 * @brief Disable PWM output on channel B
 * 
 * @param pwm The structure that contains the PWM information
 */
inline void rfs_pwm8_disable_channel_b(struct rfs_pwm8_t *pwm)
{
    rfs_timer8_set_compare_match_output_mode_b(&pwm->timer, RFS_TIMER_COMB_NORMAL);
}

/**
 * @brief Enable PWM output on channel A
 * 
 * @param pwm The structure that contains the PWM information
 */
void rfs_pwm8_enable_channel_a(struct rfs_pwm8_t *pwm);

/**
 * @brief Enable PWM output on channel B
 * 
 * @param pwm The structure that contains the PWM information
 */
void rfs_pwm8_enable_channel_b(struct rfs_pwm8_t *pwm);

/**
 * @brief Set the PWM signal frequency
 * 
 * This method does not set the exact requested frequency, but the closest frequency that can be obtained
 * using the clock prescaling values. The selected frequency will always be higher than the
 * requested frequency, except when the requested frequency is higher than the maximum frequency that can
 * be obtained.
 * 
 * The list of predefined frequencies depend on the CPU clock frequency. The predefined frequencies are those
 * obtained of dividing the CPU clock frequency by 256, 512, 2048, 4096, 16384, 32768, 65536, 131072, 262144
 * and 524288. For instance, for a CPU clock frequency of 16 MHz, the predefined PWM frequencies are:
 * 62.5 kHz, 31.25 kHz, 7812.5 Hz, 3906.25 Hz, 976.56 Hz, 488.28 Hz, 244.14 Hz, 122.07 Hz, 61.04 Hz and 30.52 Hz.
 * 
 * @param pwm The structure that contains the PWM information
 * @param frequency The requested PWM signal frequency
 * @param cpu_frequency The CPU's clock frequency
 */
void rfs_pwm8_set_frequency(struct rfs_pwm8_t *pwm, uint32_t frequency, uint32_t cpu_frequency);

/**
 * @brief Set an extact value for the PWM signal frequency
 * 
 * This method will try to set the exact requested PWM frequency. For that, first an aproximate value
 * is selected, like it is done by the rfs_pwm8_set_frequency method. Finally, the corresponding timer
 * is configured to use the value in OCRA tor the TOP value, and a suitable value for OCRA is computed
 * to obtain the requested frequency.
 * 
 * When the frequency is set using this method, it won't make sense to set a duty cycle greater than the
 * value in the OCRA register.
 * 
 * This method obtains the exact requested frequency by sacrificing resolution. The 8-bit timers offer
 * a maximum resolution of 256 different values for the duty cycle. The lower the used OCRA value,
 * the lower the resolution.
 * 
 * @param pwm The structure that contains the PWM information
 * @param frequency The requested PWM signal frequency
 * @param cpu_frequency The CPU's clock frequency
 */
void rfs_pwm8_set_frequency_exact(struct rfs_pwm8_t *pwm, uint32_t frequency, uint32_t cpu_frequency);

/**
 * @brief Set the duty cycle for channel A of the given PWM module
 * 
 * Note that this method shouldn't be used if we are using an exact frequency, because the exact
 * frequency requires the top value to be in the OCRXA register. Thus, when an exact frequency
 * is used, only channel B is available.
 * 
 * @param pwm The structure that contains the PWM information
 * @param duty_cycle The new duty cycle
 */
inline void rfs_pwm8_set_duty_cycle_channel_a(struct rfs_pwm8_t *pwm, uint8_t duty_cycle)
{
    rfs_timer8_set_ocra(&pwm->timer, duty_cycle);
}

/**
 * @brief Set the duty cycle for channel B of the given PWM module
 * 
 * @param pwm The structure that contains the PWM information
 * @param duty_cycle The new duty cycle
 */
inline void rfs_pwm8_set_duty_cycle_channel_b(struct rfs_pwm8_t *pwm, uint8_t duty_cycle)
{
    rfs_timer8_set_ocrb(&pwm->timer, duty_cycle);
}

/**
 * @brief Initialize the 16-bit PWM
 * 
 * @param pwm The structure that contains the PWM information
 * @param timer Which timer to use to control the PWM signal
 */
void rfs_pwm16_init(struct rfs_pwm16_t *pwm, enum rfs_timer16_enum timer);

/**
 * @brief Disable PWM output on channel A
 * 
 * @param pwm The structure that contains the PWM information
 */
inline void rfs_pwm16_disable_channel_a(struct rfs_pwm16_t *pwm)
{
    rfs_timer16_set_compare_match_output_mode_a(&pwm->timer, RFS_TIMER_COMA_NORMAL);
}

/**
 * @brief Disable PWM output on channel B
 * 
 * @param pwm The structure that contains the PWM information
 */
inline void rfs_pwm16_disable_channel_b(struct rfs_pwm16_t *pwm)
{
    rfs_timer16_set_compare_match_output_mode_b(&pwm->timer, RFS_TIMER_COMB_NORMAL);
}

/**
 * @brief Enable PWM output on channel A
 * 
 * @param pwm The structure that contains the PWM information
 */
void rfs_pwm16_enable_channel_a(struct rfs_pwm16_t *pwm);

/**
 * @brief Enable PWM output on channel B
 * 
 * @param pwm The structure that contains the PWM information
 */
void rfs_pwm16_enable_channel_b(struct rfs_pwm16_t *pwm);

#endif