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