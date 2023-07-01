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

#define RFS_TIMER16_CRA_MODE_MASK   0b00000011
#define RFS_TIMER16_CRB_MODE_MASK   0b00011000
#define RFS_TIMER16_CRB_CLOCK_MASK  0b00000111

void rfs_timer16_init(struct rfs_timer_t *timer, enum rfs_timers which)
{
    switch (which) {
    case RFS_TIMER1:
        timer->cra = &TCCR1A;
        timer->crb = &TCCR1B;
        timer->crc = &TCCR1C;
        timer->cnt = &TCNT1;
        timer->ocra = &OCR1A;
        timer->ocrb = &OCR1B;
        timer->icr = &ICR1;
        break;
    }
}

void rfs_timer16_setmode(struct rfs_timer_t *timer, enum rfs_timer_modes mode)
{
    *timer->cra &= ~RFS_TIMER16_CRA_MODE_MASK;
    *timer->cra |= (mode & RFS_TIMER16_CRA_MODE_MASK);
    *timer->crb &= ~RFS_TIMER16_CRB_MODE_MASK;
    *timer->crb |= ((mode << 1) & RFS_TIMER16_CRB_MODE_MASK);
}

void rfs_timer16_setclock(struct rfs_timer_t *timer, enum rfs_timer_clocks clock)
{
    *timer->crb &= ~RFS_TIMER16_CRB_CLOCK_MASK;
    *timer->crb |= (clock & RFS_TIMER16_CRB_CLOCK_MASK);
}