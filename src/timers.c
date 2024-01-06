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

void rfs_timer_init(struct rfs_timer_t *timer, enum rfs_timer_enum which)
{
    switch (which) {
    case RFS_TIMER0:
        timer->cra = &TCCR0A;
        break;
    case RFS_TIMER1:
        timer->cra = &TCCR1A;
        break;
    case RFS_TIMER2:
        timer->cra = &TCCR2A;
        break;
    }
}

void rfs_timer_set_mode_8(struct rfs_timer_t *timer, enum rfs_timer_mode_8 mode)
{
    *timer->cra = (*timer->cra & ~RFS_TIMER_CRA_MODE_MASK) | (mode & RFS_TIMER_CRA_MODE_MASK);
    *rfs_timer_crb(timer) = (*rfs_timer_crb(timer) & ~RFS_TIMER_CRB_MODE_MASK) | (mode & RFS_TIMER_CRB_MODE_MASK);
}
