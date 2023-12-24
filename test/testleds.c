/*
testleds.c - Test program for the LED subsystem.

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

#include "rfsavr/leds.h"

#include <avr/io.h>

#define LEDS_COUNT  12
#define LEDS_PORT   PORTB
#define LEDS_PIN    3

int main()
{
    struct rfs_grb_t led_values[LEDS_COUNT];
    struct rfs_pin_t leds_pin = {.port = &LEDS_PORT, .pin = LEDS_PIN};
    rfs_pin_setoutput(&leds_pin);

    for (uint8_t i = 0; i < LEDS_COUNT; i++) {
        led_values[i].green = 0;
        led_values[i].red = 0;
        led_values[i].blue = 255;
    }
    rfs_leds_write(led_values, LEDS_COUNT, &leds_pin);
}