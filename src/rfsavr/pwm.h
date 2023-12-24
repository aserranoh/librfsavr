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

enum rfs_pwm_channel {
    OC0A,
    OC0B,
    OC1A,
    OC1B,
    OC2A,
    OC2B
};

struct rfs_pwm8_t {
    struct rfs_timer8_t timer;
};

inline void rfs_pwm8_init(struct rfs_pwm8_t *pwm, enum rfs_timer8_enum timer)
{
    rfs_timer8_init(&(pwm->timer), timer);
}

/*void rfs_pwm_set_duty_cycle_8(struct rfs_pwm_t *pwm, uint8_t duty_cycle);
void rfs_pwm_set_duty_cycle_16(struct rfs_pwm_t *pwm, uint16_t duty_cycle);
void rfs_pwm_close(struct rfs_pwm_t *pwm);*/

#endif