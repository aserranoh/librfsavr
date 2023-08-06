/*
string.c - String utility functions

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

#include <rfsavr/string.h>

const char hextable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

char rfs_str_itohex(uint8_t value)
{
    return hextable[value & 0xf];
}

void rfs_str_u16tohex(uint16_t value, char *output_string)
{
    uint8_t index = value & 0xf;
    output_string[3] = hextable[index];
    index = (value >> 4) & 0xf;
    output_string[2] = hextable[index];
    index = (value >> 8) & 0xf;
    output_string[1] = hextable[index];
    index = (value >> 12) & 0xf;
    output_string[0] = hextable[index];
}