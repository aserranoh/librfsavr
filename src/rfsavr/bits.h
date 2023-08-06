/*
bits.h - Define some macros to perform bit operations on registers

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

#ifndef RFS_BITS_H
#define RFS_BITS_H

/**
 * @brief Set some bits in register given a mask
 * 
 * The bits to modify from register are the bits set at 1 on mask.
 * mask can have non-consecutive bits set to one.
 * The bits that are 0 in mask they must be 0 also in value
 * 
 * @param register The register to modify
 * @param mask The part of the register to modify (the bits in 1)
 * @param value The new bits to set in register
 */
#define rfs_bits_setmask(register, mask, value) \
    (register) &= ~(mask); \
    (register) |= (value);

/**
 * @brief If value is one, set bit on register. Otherwise, reset it.
 * 
 * @param register The register to modify
 * @param bit The bit to set in register
 * @param value New value of bit in register
 */
#define rfs_bits_setbit(register, bit, value) \
    (register) &= ~_BV(bit); \
    (register) |= ((value) << (bit));

#endif