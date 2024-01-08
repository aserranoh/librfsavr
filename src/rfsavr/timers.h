/*
timers.h - Use the timers of the AVR microcontroller.

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

#ifndef RFS_TIMERS_H
#define RFS_TIMERS_H

#include <stdint.h>
#include <avr/io.h>

#include "rfsavr/bits.h"
#include "rfsavr/io.h"
#include "rfsavr/types.h"

#define RFS_TIMER_COUNT             3
#define RFS_TIMER_CHANNELS_COUNT    2

#define RFS_TIMER_CRA_MODE_MASK     0b00000011
#define RFS_TIMER_CLOCK_MASK        0b00000111
#define RFS_TIMER_CRB_MODE_MASK     0b00011000
#define RFS_TIMER_COMA_MASK         0b11000000
#define RFS_TIMER_COMB_MASK         0b00110000

/**
 * @brief Struct that contains all the address to all the registers necessary to control the timer
 */
struct rfs_timer_t {
    volatile uint8_t *cra;
    volatile uint8_t *ocra;
    volatile uint8_t *ocrb;
};

/**
 * @brief Macros to obtain the addresses of the timer related registers from the TCCRXA register
 */
#define rfs_timer_crb(timer)        ((timer)->cra + 1)
#define rfs_timer_crc(timer)        ((timer)->cra + 2)
#define rfs_timer_cnt_8(timer)      ((timer)->cra + 2)
#define rfs_timer_cnt_16(timer)     ((timer)->cra + 4)
#define rfs_timer_icr_16(timer)     ((timer)->cra + 6)

/**
 * @brief Enumeration with the valid timers
 */
enum rfs_timer_enum {
    RFS_TIMER0,
    RFS_TIMER1,
    RFS_TIMER2
};

/**
 * @brief Enumeration for the 8-bit timer modes
 */
enum rfs_timer_mode_8 {
    RFS_TIMER8_MODE_NORMAL                  = 0b00000,
    RFS_TIMER8_MODE_PWM_PHASE_CORRECT       = 0b00001,
    RFS_TIMER8_MODE_CTC                     = 0b00010,
    RFS_TIMER8_MODE_FAST_PWM                = 0b00011,
    RFS_TIMER8_MODE_PWM_PHASE_CORRECT_OCRA  = 0b01001,
    RFS_TIMER8_MODE_FAST_PWM_OCRA           = 0b01011,
};

/**
 * @brief Enumeration for the 16-bit timer modes
 */
enum rfs_timer_mode_16 {
    RFS_TIMER16_MODE_NORMAL                             = 0b00000,
    RFS_TIMER16_MODE_PWM_PHASE_CORRECT_8                = 0b00001,
    RFS_TIMER16_MODE_PWM_PHASE_CORRECT_9                = 0b00010,
    RFS_TIMER16_MODE_PWM_PHASECORRECT_10                = 0b00011,
    RFS_TIMER16_MODE_CTC_OCRA                           = 0b01000,
    RFS_TIMER16_MODE_FAST_PWM_8                         = 0b01001,
    RFS_TIMER16_MODE_FAST_PWM_9                         = 0b01010,
    RFS_TIMER16_MODE_FAST_PWM_10                        = 0b01011,
    RFS_TIMER16_MODE_PWM_PHASE_FREQUENCY_CORRECT_ICR    = 0b10000,
    RFS_TIMER16_MODE_PWM_PHASE_FREQUENCY_CORRECT_OCRA   = 0b10001,
    RFS_TIMER16_MODE_PWM_PHASE_CORRECT_ICR              = 0b10010,
    RFS_TIMER16_MODE_PWM_PHASE_CORRECT_OCRA             = 0b10011,
    RFS_TIMER16_MODE_CTC_ICR                            = 0b11000,
    RFS_TIMER16_MODE_FAST_PWM_ICR                       = 0b11010,
    RFS_TIMER16_MODE_FAST_PWM_OCRA                      = 0b11011,
};

/**
 * @brief Enumeration that contain the possible clock divisor values for a timer
 */
enum rfs_timer_clock {
    RFS_TIMER_CLOCK_NONE                = 0,
    RFS_TIMER0_CLOCK_1                  = 1,
    RFS_TIMER0_CLOCK_8                  = 2,
    RFS_TIMER0_CLOCK_64                 = 3,
    RFS_TIMER0_CLOCK_256                = 4,
    RFS_TIMER0_CLOCK_1024               = 5,
    RFS_TIMER0_CLOCK_EXTERNAL_FALLING   = 6,
    RFS_TIMER0_CLOCK_EXTERNAL_RAISING   = 7,
    RFS_TIMER2_CLOCK_1                  = 1,
    RFS_TIMER2_CLOCK_8                  = 2,
    RFS_TIMER2_CLOCK_32                 = 3,
    RFS_TIMER2_CLOCK_64                 = 4,
    RFS_TIMER2_CLOCK_128                = 5,
    RFS_TIMER2_CLOCK_256                = 6,
    RFS_TIMER2_CLOCK_1024               = 7,
};

/**
 * @brief Enumeration with the possible output modes on compare match for channel A
 */
enum rfs_timer_com_a {
    RFS_TIMER_COMA_NORMAL    = 0b00000000,
    RFS_TIMER_COMA_TOGGLE    = 0b01000000,
    RFS_TIMER_COMA_CLEAR     = 0b10000000,
    RFS_TIMER_COMA_SET       = 0b11000000,
    RFS_TIMER_COMA_NONINVERT = 0b10000000,
    RFS_TIMER_COMA_INVERT    = 0b11000000,
};

/**
 * @brief Enumeration with the possible output modes on compare match for channel B
 */
enum rfs_timer_com_b {
    RFS_TIMER_COMB_NORMAL    = 0b00000000,
    RFS_TIMER_COMB_TOGGLE    = 0b00010000,
    RFS_TIMER_COMB_CLEAR     = 0b00100000,
    RFS_TIMER_COMB_SET       = 0b00110000,
    RFS_TIMER_COMB_NONINVERT = 0b00100000,
    RFS_TIMER_COMB_INVERT    = 0b00110000,
};

/**
 * @brief The lists of clock divisor for the different timers
 */
extern struct rfs_list_u16_t RFS_TIMER_DIVISOR_TABLE[3];

/**
 * @brief Matrix of outputs by timer and channel
 */
extern struct rfs_pin_t RFS_TIMER_COMPARE_OUTPUT[RFS_TIMER_COUNT][RFS_TIMER_CHANNELS_COUNT];

/**
 * @brief Macro to return the compare output pin for the given timer and channel
 */
#define rfs_timer_compare_output(timer, channel)    RFS_TIMER_COMPARE_OUTPUT[timer][channel]

/**
 * @brief Macro to return the divisor table for the given timer
 */
#define rfs_timer_divisor_table(timer)  RFS_TIMER_DIVISOR_TABLE[timer]

/**
 * @brief Initialize the timer
 * 
 * @param timer The structure that contains the timer information
 * @param which Which timer to use
 */
void rfs_timer_init(struct rfs_timer_t *timer, enum rfs_timer_enum which);

/**
 * @brief Return the timer counter value (8-bit)
 * 
 * @param timer The structure that contains the timer information
 * 
 * @returns The timer counter value
 */
inline uint8_t rfs_timer_get_8(struct rfs_timer_t *timer)
{
    return *rfs_timer_cnt_8(timer);
}

/**
 * @brief Return the timer counter value (16-bit)
 * 
 * @param timer The structure that contains the timer information
 * 
 * @returns The timer counter value
 */
inline uint16_t rfs_timer_get_16(struct rfs_timer_t *timer)
{
    return *rfs_timer_cnt_16(timer);
}

/**
 * @brief Return the current mode of the timer
 * 
 * @param timer The structure that contains the timer information
 */
inline int8_t rfs_timer_get_mode(struct rfs_timer_t *timer)
{
    return (*timer->cra & RFS_TIMER_CRA_MODE_MASK) | (*rfs_timer_crb(timer) & RFS_TIMER_CRB_MODE_MASK);
}

/**
 * @brief Set the timer counter value (8-bit)
 * 
 * @param timer The structure that contains the timer information
 * @param value The new counter value
 */
inline void rfs_timer_set_8(struct rfs_timer_t *timer, uint8_t value)
{
    *rfs_timer_cnt_8(timer) = value;
}

/**
 * @brief Set the timer counter value (16-bit)
 * 
 * @param timer The structure that contains the timer information
 * @param value The new counter value
 */
inline void rfs_timer_set_16(struct rfs_timer_t *timer, uint16_t value)
{
    *rfs_timer_cnt_16(timer) = value;
}

/**
 * @brief Set the timer clock source
 * 
 * @param timer The structure that contains the timer information
 * @param clock The new clock source
 */
inline void rfs_timer_set_clock(struct rfs_timer_t *timer, enum rfs_timer_clock clock)
{
    rfs_bits_set_mask(*rfs_timer_crb(timer), RFS_TIMER_CLOCK_MASK, clock);
}

/**
 * @brief Set the output mode for channel A on compare match
 * 
 * @param timer The structure that contains the timer information
 * @param mode The channel A pin mode on compare match
 */
inline void rfs_timer_set_compare_match_output_mode_a(struct rfs_timer_t *timer, enum rfs_timer_com_a mode)
{
    rfs_bits_set_mask(*timer->cra, RFS_TIMER_COMA_MASK, mode);
}

/**
 * @brief Set the output mode for channel B on compare match
 * 
 * @param timer The structure that contains the timer information
 * @param mode The channel B pin mode on compare match
 */
inline void rfs_timer_set_compare_match_output_mode_b(struct rfs_timer_t *timer, enum rfs_timer_com_b mode)
{
    rfs_bits_set_mask(*timer->cra, RFS_TIMER_COMB_MASK, mode);
}

/**
 * @brief Set the timer mode (8-bit)
 * 
 * @param timer The structure that contains the timer information
 * @param mode The timer working mode
 */
void rfs_timer_set_mode_8(struct rfs_timer_t *timer, enum rfs_timer_mode_8 mode);

/**
 * @brief Set the timer mode (16-bit)
 * 
 * @param timer The structure that contains the timer information
 * @param mode The timer working mode
 */
inline void rfs_timer_set_mode_16(struct rfs_timer_t *timer, enum rfs_timer_mode_16 mode)
{
    rfs_timer_set_mode_8(timer, mode);
}

/**
 * @brief Set the Output Compare value for channel A (OCRA), for an 8-bit timer
 * 
 * @param timer The structure that contains the timer information
 * @param mode The new OCRA value
 */
inline void rfs_timer_set_ocra_8(struct rfs_timer_t *timer, uint8_t ocra)
{
    *timer->ocra = ocra;
}

/**
 * @brief Set the Output Compare value for channel A (OCRA), for a 16-bit timer
 * 
 * @param timer The structure that contains the timer information
 * @param value The new OCRA value
 */
inline void rfs_timer_set_ocra_16(struct rfs_timer_t *timer, uint16_t value)
{
    *(uint16_t *)timer->ocra = value;
}

/**
 * @brief Set the Output Compare value for channel B (OCRB), for an 8-bit timer
 * 
 * @param timer The structure that contains the timer information
 * @param mode The new OCRB value
 */
inline void rfs_timer_set_ocrb_8(struct rfs_timer_t *timer, uint8_t ocrb)
{
    *timer->ocrb = ocrb;
}

/**
 * @brief Set the Output Compare value for channel B (OCRB), for a 16-bit timer
 * 
 * @param timer The structure that contains the timer information
 * @param value The new OCRB value
 */
inline void rfs_timer_set_ocrb_16(struct rfs_timer_t *timer, uint16_t value)
{
    *(uint16_t *)timer->ocrb = value;
}

/**
 * @brief Return whether the OCA flag is active
 * 
 * @param timer The structure that contains the timer information
 * 
 * @returns Whether the OCA flag is active
 */
/*inline int8_t rfs_timer16_get_oca_flag(struct rfs_timer16_t *timer)
{
    return *timer->ifr & ~_BV(OCF1A);
}*/

/**
 * @brief Reset the OCA flag
 * 
 * The OCA flag is reset by writing a logic 1 to that bit.
 * 
 * @param timer The structure that contains the timer information
 */
/*inline void rfs_timer16_reset_oca_flag(struct rfs_timer_t *timer)
{
    *timer->ifr |= _BV(OCF1A);
}*/

#endif