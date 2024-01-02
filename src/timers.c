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

void rfs_timer8_init(struct rfs_timer8_t *timer, enum rfs_timer8_enum which)
{
    switch (which) {
    case RFS_TIMER0:
        timer->cra = &TCCR0A;
        break;
    case RFS_TIMER2:
        timer->cra = &TCCR2A;
        break;
    }
}

void rfs_timer8_set_mode(struct rfs_timer8_t *timer, enum rfs_timer8_mode mode)
{
    *timer->cra = (*timer->cra & ~RFS_TIMER8_CRA_MODE_MASK) | (mode & RFS_TIMER8_CRA_MODE_MASK);
    *rfs_timer8_crb(timer) = (*rfs_timer8_crb(timer) & ~RFS_TIMER8_CRB_MODE_MASK) | (mode & RFS_TIMER8_CRB_MODE_MASK);
}

void rfs_timer16_init(struct rfs_timer16_t *timer, enum rfs_timer16_enum which)
{
    timer->cra = &TCCR1A;
}

void rfs_timer16_set_mode(struct rfs_timer16_t *timer, enum rfs_timer16_mode mode)
{
    *timer->cra = (*timer->cra & ~RFS_TIMER16_CRA_MODE_MASK) | (mode & RFS_TIMER16_CRA_MODE_MASK);
    *rfs_timer16_crb(timer) = (*rfs_timer16_crb(timer) & ~RFS_TIMER16_CRB_MODE_MASK) | (mode & RFS_TIMER16_CRB_MODE_MASK);
}
