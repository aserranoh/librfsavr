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

#include <rfsavr/timers.h>

static uint16_t RFS_TIMER_DIVISOR_TABLE_0[] = {1, 8, 64, 256, 1024};
static uint16_t RFS_TIMER_DIVISOR_TABLE_2[] = {1, 8, 32, 64, 128, 256, 1024};

const struct rfs_list_u16_t RFS_TIMER_DIVISOR_TABLE[3] = {
    {RFS_TIMER_DIVISOR_TABLE_0, 5},
    {RFS_TIMER_DIVISOR_TABLE_0, 5},
    {RFS_TIMER_DIVISOR_TABLE_2, 7}
};

const struct rfs_pin_t RFS_TIMER_COMPARE_OUTPUT[RFS_TIMER_COUNT][RFS_TIMER_CHANNELS_COUNT] = {
    {{&PORTD, 6}, {&PORTD, 5}},
    {{&PORTB, 1}, {&PORTB, 2}},
    {{&PORTB, 3}, {&PORTD, 3}}
};

void rfs_timer_init(struct rfs_timer_t *timer, enum rfs_timer_enum which)
{
    switch (which) {
    case RFS_TIMER0:
        timer->cra = &TCCR0A;
        timer->ocra8 = &OCR0A;
        timer->ocrb8 = &OCR0B;
        break;
    case RFS_TIMER1:
        timer->cra = &TCCR1A;
        timer->ocra16 = &OCR1A;
        timer->ocrb16 = &OCR1B;
        break;
    case RFS_TIMER2:
        timer->cra = &TCCR2A;
        timer->ocra8 = &OCR2A;
        timer->ocrb8 = &OCR2B;
        break;
    }
}

void rfs_timer_set_mode_8(const struct rfs_timer_t *timer, enum rfs_timer_mode_8 mode)
{
    rfs_bits_set_mask(*timer->cra, RFS_TIMER_CRA_MODE_MASK, mode & RFS_TIMER_CRA_MODE_MASK);
    rfs_bits_set_mask(*rfs_timer_crb(timer), RFS_TIMER_CRB_MODE_MASK, mode & RFS_TIMER_CRB_MODE_MASK);
}
