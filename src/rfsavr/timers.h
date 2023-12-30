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

#define RFS_TIMER8_COMA_MASK        0b11000000
#define RFS_TIMER8_COMB_MASK        0b00110000
#define RFS_TIMER8_CLOCK_MASK       0b00000111
#define RFS_TIMER8_CRA_MODE_MASK    0b00000011
#define RFS_TIMER8_CRB_MODE_MASK    0b00001000

/**
 * @brief Struct that contains all the address to all the registers necessary to control the 8-bit timer
 */
struct rfs_timer8_t {
    volatile uint8_t *cra;
    volatile uint8_t *crb;
    volatile uint8_t *cnt;
    volatile uint8_t *ocra;
    volatile uint8_t *ocrb;
    volatile uint8_t *ifr;
};

/**
 * @brief Struct that contains all the address to all the registers necessary to control the timer
 */
struct rfs_timer_t {
    volatile uint8_t *cra;
    volatile uint8_t *crb;
    volatile uint8_t *crc;
    volatile uint16_t *cnt;
    volatile uint16_t *ocra;
    volatile uint16_t *ocrb;
    volatile uint16_t *icr;
    volatile uint8_t *ifr;
};

enum rfs_timer8_clock {
    RFS_TIMER0_CLOCK_NONE = 0,
    RFS_TIMER0_CLOCK_1 = 1,
    RFS_TIMER0_CLOCK_8 = 2,
    RFS_TIMER0_CLOCK_64 = 3,
    RFS_TIMER0_CLOCK_256 = 4,
    RFS_TIMER0_CLOCK_1024 = 5,
    RFS_TIMER0_EXTERNAL_FALLING = 6,
    RFS_TIMER0_EXTERNAL_RAISING = 7,
    RFS_TIMER2_CLOCK_NONE = 0,
    RFS_TIMER2_CLOCK_1 = 1,
    RFS_TIMER2_CLOCK_8 = 2,
    RFS_TIMER2_CLOCK_32 = 3,
    RFS_TIMER2_CLOCK_64 = 4,
    RFS_TIMER2_CLOCK_128 = 5,
    RFS_TIMER2_CLOCK_256 = 6,
    RFS_TIMER2_CLOCK_1024 = 7,
};

/**
 * @brief Enumeration with the 8-bit timers
 */
enum rfs_timer8_enum {
    RFS_TIMER0,
    RFS_TIMER2
};

/**
 * @brief Enumeration for the 8-bit timer modes
 */
enum rfs_timer8_mode {
    RFS_TIMER8_MODE_NORMAL = 0,
    RFS_TIMER8_MODE_PWM_PHASE_CORRECT = 1,
    RFS_TIMER8_MODE_CTC = 2,
    RFS_TIMER8_MODE_FAST_PWM = 3,
    RFS_TIMER8_MODE_PWM_PHASE_CORRECT_OCRA = 9,
    RFS_TIMER8_MODE_FAST_PWM_OCRA = 11,
};

/**
 * @brief Enumeration with all the timers on the system
 */
enum rfs_timers {
    RFS_TIMER1
};

/**
 * @brief Enumeration with the timer modes
 */
enum rfs_timer_modes {
    RFS_TIMER16_NORMAL,
    RFS_TIMER16_PWM_PHASECORRECT_8,
    RFS_TIMER16_PWM_PHASECORRECT_9,
    RFS_TIMER16_PWM_PHASECORRECT_10,
    RFS_TIMER16_CTC_OCR1A,
};

/**
 * @brief Enumeration with the possible clock prescaling values
 */
enum rfs_timer_clocks {
    RFS_TIMER16_NONE,
    RFS_TIMER16_1,
    RFS_TIMER16_8,
    RFS_TIMER16_64,
    RFS_TIMER16_256,
    RFS_TIMER16_1024,
    RFS_TIMER16_FALLING,
    RFS_TIMER16_RAISING
};

/**
 * @brief Enumeration with the possible output modes on compare match for channel A
 */
enum rfs_timer_comA {
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
enum rfs_timer_comB {
    RFS_TIMER_COMB_NORMAL    = 0b00000000,
    RFS_TIMER_COMB_TOGGLE    = 0b00010000,
    RFS_TIMER_COMB_CLEAR     = 0b00100000,
    RFS_TIMER_COMB_SET       = 0b00110000,
    RFS_TIMER_COMB_NONINVERT = 0b00100000,
    RFS_TIMER_COMB_INVERT    = 0b00110000,
};

/**
 * @brief Initialize the 8-bit timer
 * 
 * @param timer The structure that contains the timer information
 * @param which Which timer to use
 */
void rfs_timer8_init(struct rfs_timer8_t *timer, enum rfs_timer8_enum which);

inline enum rfs_timer8_mode rfs_timer8_get_mode(struct rfs_timer8_t *timer)
{
    return (*timer->cra & RFS_TIMER8_CRA_MODE_MASK) | (*timer->crb & RFS_TIMER8_CRB_MODE_MASK);
}

inline void rfs_timer8_set_clock(struct rfs_timer8_t *timer, enum rfs_timer8_clock clock)
{
    *timer->crb = (*timer->crb & ~RFS_TIMER8_CLOCK_MASK) | clock;
}

/**
 * @brief Set the output mode for channel A on compare match
 * 
 * @param timer The structure that contains the timer information
 * @param mode The channel A pin mode on compare match
 */
inline void rfs_timer8_set_compare_match_output_mode_A(struct rfs_timer8_t *timer, enum rfs_timer_comA mode)
{
    *timer->cra = ((*timer->cra & ~RFS_TIMER8_COMA_MASK) | mode);
}

/**
 * @brief Set the output mode for channel B on compare match
 * 
 * @param timer The structure that contains the timer information
 * @param mode The channel B pin mode on compare match
 */
inline void rfs_timer8_set_compare_match_output_mode_B(struct rfs_timer8_t *timer, enum rfs_timer_comB mode)
{
    *timer->cra = ((*timer->cra & ~RFS_TIMER8_COMB_MASK) | mode);
}

/**
 * @brief Set the mode for the timer
 * 
 * @param timer The structure that contains the timer information
 * @param mode The timer working mode
 */
void rfs_timer8_set_mode(struct rfs_timer8_t *timer, enum rfs_timer8_mode mode);

inline void rfs_timer8_set_ocra(struct rfs_timer8_t *timer, uint8_t ocra)
{
    *timer->ocra = ocra;
}

/**
 * @brief Return the timer counter value
 * 
 * @param timer The structure that contains the timer information
 * 
 * @returns The timer counter value
 */
inline uint16_t rfs_timer16_get(struct rfs_timer_t *timer)
{
    return *timer->cnt;
}

/**
 * @brief Return whether the OCA flag is active
 * 
 * @param timer The structure that contains the timer information
 * 
 * @returns Whether the OCA flag is active
 */
inline int8_t rfs_timer16_get_oca_flag(struct rfs_timer_t *timer)
{
    return *timer->ifr & ~_BV(OCF1A);
}

/**
 * @brief Initialize the timer
 * 
 * @param timer The structure that contains the timer information
 * @param which Which timer to use
 */
void rfs_timer16_init(struct rfs_timer_t *timer, enum rfs_timers which);

/**
 * @brief Reset the OCA flag
 * 
 * The OCA flag is reset by writing a logic 1 to that bit.
 * 
 * @param timer The structure that contains the timer information
 */
inline void rfs_timer16_reset_oca_flag(struct rfs_timer_t *timer)
{
    *timer->ifr |= _BV(OCF1A);
}

/**
 * @brief Set the timer counter value
 * 
 * @param timer The structure that contains the timer information
 * @param value The new counter value
 */
inline void rfs_timer16_set(struct rfs_timer_t *timer, uint16_t value)
{
    *timer->cnt = value;
}

/**
 * @brief Set the timer clock prescaler
 * 
 * @param timer The structure that contains the timer information
 * @param clock The clock prescaler value
 */
void rfs_timer16_setclock(struct rfs_timer_t *timer, enum rfs_timer_clocks clock);

/**
 * @brief Set the timer mode
 * 
 * @param timer The structure that contains the timer information
 * @param mode The timer mode
 */
void rfs_timer16_setmode(struct rfs_timer_t *timer, enum rfs_timer_modes mode);

/**
 * @brief Set a new value on the OCRA register
 * 
 * @param timer The structure that contains the timer information
 * @param value The new OCRA register value
 */
inline void rfs_timer16_setocra(struct rfs_timer_t *timer, uint16_t value)
{
    *timer->ocra = value;
}

/**
 * @brief Set a new value on the OCRB register
 * 
 * @param timer The structure that contains the timer information
 * @param value The new OCRB register value
 */
inline void rfs_timer16_setocrb(struct rfs_timer_t *timer, uint16_t value)
{
    *timer->ocra = value;
}

#endif