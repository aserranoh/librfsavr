/*
io.h - Use IO ports of the AVR microcontroller.

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

#include "rfsavr/io.h"

void rfs_pin_init(struct rfs_pin_t *pin, volatile uint8_t *port, int8_t pin_number)
{
    pin->port = port;
    pin->pin = pin_number;
}

void rfs_pin_set_input(const struct rfs_pin_t *pin)
{
    rfs_pin_zero(rfs_ddr(pin), pin->pin);
    rfs_pin_zero(pin->port, pin->pin);
}

void rfs_pin_set_input_pullup(const struct rfs_pin_t *pin)
{
    rfs_pin_zero(rfs_ddr(pin), pin->pin);
    rfs_pin_one(pin->port, pin->pin);
}