/*
string.h - String utility functions

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

#include <stdint.h>

/**
 * @brief Transform an integer value to an hexadecimal character
 * 
 * The integer value has range from 0 to 15
 * 
 * @param value The integer value
 * 
 * @returns The integer value as an hexadecimal character
 */
char rfs_str_itohex(uint8_t value);

/**
 * @brief Converts a 16 bit unsigned integer to hexadecimal and put the result in the given buffer
 * 
 * @param value The integer value to convert to hexadecimal
 * @param output_string The buffer where to put the hexadecimal string
 */
void rfs_str_u16tohex(uint16_t value, char *output_string);