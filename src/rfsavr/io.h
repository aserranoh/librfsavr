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

#ifndef RFS_IO_H
#define RFS_IO_H

#include <avr/io.h>

/**
 * @brief Struct that contains all the information to use a IO pin
 */
struct rfs_pin_t {
    volatile uint8_t *port;
    int8_t pin;
};

/**
 * @brief Macro to obtain the address of the DDR register related with a given PORT register address
 */
#define rfs_ddr(pin)    ((pin)->port - 1)

/**
 * @brief Macro to obtain the address of the PIN register related with a given PORT register address
 */
#define rfs_pin(pin)    ((pin)->port - 2)

/**
 * @brief Macro to reset a given pin of a given register
 */
#define rfs_pin_zero(register, pin) *(register) &= ~_BV(pin)

/**
 * @brief Macro to set a given pin of a given register
 */
#define rfs_pin_one(port, pin)  *(port) |= _BV(pin)

/**
 * @brief Initialize the pin structure
 * 
 * @param pin The pin structure
 * @param port The port where the pin is located
 * @param pin_number The pin number
 */
void rfs_pin_init(struct rfs_pin_t *pin, volatile uint8_t *port, int8_t pin_number);

/**
 * @brief Configure the given pin as input
 * 
 * @param pin The pin to configure as input
 */
void rfs_pin_set_input(const struct rfs_pin_t *pin);

/**
 * @brief Configure the given pin as input and enable the pull-up resistor
 * 
 * @param pin The pin to configure as input
 */
void rfs_pin_set_input_pullup(const struct rfs_pin_t *pin);

/**
 * @brief Configure the given pin as output
 * 
 * @param pin The pin to configure as output
 */
inline void rfs_pin_set_output(const struct rfs_pin_t *pin)
{
    rfs_pin_one(rfs_ddr(pin), pin->pin);
}

/**
 * @brief Read the pin value
 * 
 * @param pin The pin to read
 * 
 * @return Zero if the pin is not active, or a value different than zero otherwise
 */
inline uint8_t rfs_pin_read(const struct rfs_pin_t *pin)
{
    return *rfs_pin(pin) & _BV(pin->pin);
}

/**
 * @brief Set the value of the given pin
 * 
 * @param pin The pin to set
 */
inline void rfs_pin_set(const struct rfs_pin_t *pin)
{
    rfs_pin_one(pin->port, pin->pin);
}

/**
 * @brief Reset the value of the given pin
 * 
 * @param pin The pin to reset
 */
inline void rfs_pin_reset(const struct rfs_pin_t *pin)
{
    rfs_pin_zero(pin->port, pin->pin);
}

/**
 * @brief Read the value of the given pin at the PORT register
 * 
 * @param pin The pin to read at the PORT register
 */
inline uint8_t rfs_pin_readport(const struct rfs_pin_t *pin)
{
    return *pin->port & _BV(pin->pin);
}

/**
 * @brief Toggle the value of the given pin at the PORT register
 * 
 * @param pin The pin to toggle at the PORT register
 */
inline void rfs_pin_toggle(const struct rfs_pin_t *pin)
{
    rfs_pin_one(rfs_ddr(pin), pin->pin);
}

#endif
