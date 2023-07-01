/*
leds.c - Contains some routines to interface with LED strips.

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

#include <rfsavr/leds.h>


///////////////////////////////////////////////// PUBLIC FUNCTIONS ///////////////////////////////////////////////////

void rfs_leds_write(const struct rfs_grb_t *leds_values, uint8_t leds_count, struct rfs_pin_t *leds_pin)
{
    const uint8_t port_value = *leds_pin->port;
    const uint8_t port_high = port_value | _BV(leds_pin->pin);
    const uint8_t port_low = port_value & ~_BV(leds_pin->pin);
    const uint8_t *color_ptr = (const uint8_t *)leds_values;
    const uint8_t color_value = *(color_ptr++);
    leds_count *= 3;
    uint8_t bit_mask = 0x80;

    asm (
        "set_high:" "\n\t"
            "st %a0, %1" "\n\t"
            "nop" "\n\t"
            "mov __tmp_reg__, %2" "\n\t"
            "and __tmp_reg__, %3" "\n\t"
            "breq write_zero" "\n\t"

            "lsr %3" "\n\t"
            "brne more_bits_one" "\n\t"

            "dec %4" "\n\t"
            "breq last_color_one" "\n\t"

            "ld %2, %a5+" "\n\t"
            "ldi %3, 0x80" "\n\t"

        "set_low:" "\n\t"
            "st %a0, %6" "\n\t"
            "nop" "\n\t"
            "nop" "\n\t"
            "nop" "\n\t"
            "rjmp set_high" "\n\t"

        "more_bits_one:" "\n\t"
            "nop" "\n\t"
            "nop" "\n\t"
            "rjmp set_low" "\n\t"

        "last_color_one:" "\n\t"
            "nop" "\n\t"
            "nop" "\n\t"
            "st %a0, %6" "\n\t"
            "rjmp end" "\n\t"

        "write_zero:" "\n\t"
            "st %a0, %6" "\n\t"
            "nop" "\n\t"
            "nop" "\n\t"
            "lsr %3" "\n\t"
            "brne more_bits_zero" "\n\t"

            "dec %4" "\n\t"
            "breq end" "\n\t"

            "ld %2, %a5+" "\n\t"
            "ldi %3, 0x80" "\n\t"
            "rjmp set_high" "\n\t"

        "more_bits_zero:" "\n\t"
            "nop" "\n\t"
            "nop" "\n\t"
            "nop" "\n\t"
            "nop" "\n\t"
            "rjmp set_high" "\n\t"
            
        "end:" "\n\t"
        :
        : "e" (leds_pin->port), "r" (port_high), "r" (color_value), "r" (bit_mask), "r" (leds_count), "e" (color_ptr), "r" (port_low)
    );
}