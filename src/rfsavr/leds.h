/*
leds.h - Contains some routines to interface with LED strips.

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

#ifndef RFS_LEDS_H
#define RFS_LEDS_H

#include <stdint.h>
#include <rfsavr/io.h>

/**
 * @brief Struct that contains the color value of a LED.
 */
struct rfs_grb_t {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
};

/**
 * @brief Writes a sequence of LED values to an output pin using the WS2812B serial protocol.
 *
 * @param leds_values The colors of the LEDs to write.
 * @param leds_count The number of LEDs to write.
 * @param leds_pin The pin where to write the LEDs new colors.
 */
void rfs_leds_write(const struct rfs_grb_t *leds_values, uint8_t leds_count, struct rfs_pin_t *leds_pin);

#endif