/*
adc.c - Use the ADC subsystem of the AVR microcontroller.

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

#include <rfsavr/adc.h>

int8_t rfs_adc_get8(uint8_t *result)
{
    const int8_t conversion_done = (ADCSRA & _BV(ADIF));
    if (conversion_done) {
        // Read the conversion result
        *result = ADCH;

        // Reset the interrupt flag
        ADCSRA |= _BV(ADIF);
    }
    return conversion_done;
}

int8_t rfs_adc_get16(uint16_t *result)
{
    const int8_t conversion_done = (ADCSRA & _BV(ADIF));
    if (conversion_done) {
        // Read the conversion result
        *result = ADC;

        // Reset the interrupt flag
        ADCSRA |= _BV(ADIF);
    }
    return conversion_done;
}