/*
types.h - Define some useful types

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
 * @brief A list of unsigned 16-bit integers
 */
struct rfs_list_u16_t {
    uint16_t *elements;
    uint8_t size;
};

/**
 * @brief Return the element at the given index
 * 
 * @param list The list
 * @param index The index
 * @return The element at the given index
 */
#define rfs_list_get(list,  index)  (list).elements[index]

/**
 * @brief Return the element at the given index
 * 
 * @param list The list
 * @param index The index
 * @return The list's size
 */
#define rfs_list_size(list) (list).size