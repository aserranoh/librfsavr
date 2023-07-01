/*
usart.c - Interface with the USART module of the AVR processors.

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

#include <avr/io.h>

#include <rfsavr/usart.h>
#include <rfsavr/errno.h>

/**
 * Masks used to initialize the control registers with the flags and mode.
 */
#define RFS_USART_MODE_MASK 0b11000000
#define RFS_USART_A_MASK    0b00000001
#define RFS_USART_B_MASK    0b00011100
#define RFS_USART_C_MASK    0b00111111


///////////////////////////////////////////////// PRIVATE TYPES ///////////////////////////////////////////////////


/**
 * The two possible values for the clock divisor when computing the USART baudrate.
 */
enum rfs_usart_clockdivisor
{
    RFS_USART_NORMAL_SPEED = 16,
    RFS_USART_DOUBLE_SPEED = 8
};


///////////////////////////////////////////////// PRIVATE FUNCTIONS ///////////////////////////////////////////////////


/**
 * Compute tue UBRR register value given a baudrate.
 *
 * The UBRR register value depends on the desired baudrate, the CPU frequency and the clock divisor.
 * The formula is:
 * 
 *     UBRR = (F_CPU / (16 * baudrate)) - 1
 * 
 * The division is performed in an iterative way, and the value of UBRR is found such as the error in the given baudrate
 * and the desired one is minimized.
 * On output, baudrate_error parameter contains the error between the desired baudrate and the obtained baudrate.
 *
 * @param baudrate The desired baudrate.
 * @param cpu_frequency The CPU frequency.
 * @param clock_divisor Clock divisor applied to the CPU frequency.
 * @param baudrate_error At exit, contains the error between the desired baudrate and the obtained baudrate
 *                       using the given UBRR value.
 * 
 * @returns The value to write on the UBRR register that obtains the closest USART transmission baudrate to
 *          the specified baudrate.
 */
static uint16_t
rfs_usart_getubrr (enum rfs_usart_baudrate baudrate, uint32_t cpu_frequency, enum rfs_usart_clockdivisor clock_divisor, uint32_t *baudrate_error)
{
    uint16_t ubrr = 0;
    uint32_t baudrate_sum = 0;
    uint32_t divisor = baudrate * clock_divisor;
    
    // Check that the conditions are met to calculate the UBRR value
    if (cpu_frequency < divisor) {
        return 0;
    }

    /* Search for the result of dividing cpu_frequency by divisor.
       The loop will exit so that:

       divisor * (ubrr - 1) < cpu_frequency <= divisor * ubrr
    */
    while (baudrate_sum < cpu_frequency) {
        baudrate_sum += divisor;
        ubrr++;
    }

    // Check which division value gives less error
    if ((baudrate_sum - cpu_frequency) < (divisor >> 1)) {
        // The current result is the closest one to the real result
        // Subtract one from UBRR (see formula in the header)
        ubrr--;
        *baudrate_error = baudrate_sum - cpu_frequency;
    } else {
        // The closest result was adding one less time the divisor
        ubrr -= 2;
        *baudrate_error = cpu_frequency - (baudrate_sum - divisor);
    }
    return ubrr;
}


///////////////////////////////////////////////// PUBLIC FUNCTIONS ///////////////////////////////////////////////////


void
rfs_usart_open (struct rfs_usart *usart, enum rfs_usart_device device, enum rfs_usart_mode mode, int32_t flags)
{
    // Set the USART device
    if (device == RFS_USART_0) {
        usart->udr = &UDR0;
        usart->ucsra = &UCSR0A;
        usart->ucsrb = &UCSR0B;
        usart->ucsrc = &UCSR0C;
        usart->ubrr = &UBRR0;
    }

    // Set USART mode
    if (mode == RFS_USART_SYNCMASTER) {
        DDRD |= _BV(DDD4);
    } else if (mode == RFS_USART_SYNCSLAVE) {
        DDRD &= ~_BV(DDD4);
    }
    *(usart->ucsrc) &= ~RFS_USART_MODE_MASK;
    *(usart->ucsrc) |= (mode & RFS_USART_MODE_MASK);

    // Set configuration registers
    *(usart->ucsra) &= ~RFS_USART_A_MASK;
    *(usart->ucsra) |= (flags & 0xff);
    *(usart->ucsrb) &= ~RFS_USART_B_MASK;
    *(usart->ucsrb) |= ((flags >> 8) & 0xff);
    *(usart->ucsrc) &= ~RFS_USART_C_MASK;
    *(usart->ucsrc) |= ((flags >> 16) & 0xff);
}


void
rfs_usart_setspeed (struct rfs_usart *usart, enum rfs_usart_baudrate baudrate, uint32_t cpu_frequency)
{
    uint32_t normal_speed_error;
    uint32_t double_speed_error;

    // Compute the UBRR register using normal and double speed, and check the error that each of them gives.
    uint16_t normal_speed_ubrr = rfs_usart_getubrr(baudrate, cpu_frequency, RFS_USART_NORMAL_SPEED, &normal_speed_error);
    uint16_t double_speed_ubrr = rfs_usart_getubrr(baudrate, cpu_frequency, RFS_USART_DOUBLE_SPEED, &double_speed_error);

    // Use the UBRR value that gives less error
    if (normal_speed_error <= double_speed_error) {
        *(usart->ubrr) = normal_speed_ubrr;
        *(usart->ucsra) &= ~_BV(U2X0);
    } else {
        *(usart->ubrr) = double_speed_ubrr;
        *(usart->ucsra) |= _BV(U2X0);
    }
}


int8_t
rfs_usart_read (struct rfs_usart *usart, char *data)
{
    // Check if a new byte has been received
    if ((*(usart->ucsra) & _BV(RXC0)) == 0) {
        // Return 0 to indicate that there's no data
        return 0;
    }

    // Byte received. Read the reception status and the byte itself
    uint8_t status = *(usart->ucsra);
    *data = *(usart->udr);

    // Check for error status
    if (status & (_BV(FE0) | _BV(DOR0) | _BV(UPE0)) == 0) {
        // No error
        // Return 1 to indicate that there's data
        return 1;
    }

    // Error
    if (status & _BV(FE0)) {
        // Frame error
        rfs_errno = RFS_EFRAME;
    } else if (status & _BV(DOR0)) {
        // Buffer overrun
        rfs_errno = RFS_EOVERRUN;
    } else {
        // Parity error
        rfs_errno = RFS_EPARITY;
    }
    // Return -1 to indicate that an error has occurred
    return -1;
}


int8_t
rfs_usart_write (struct rfs_usart *usart, char data)
{
    if (*(usart->ucsra) & _BV(UDRE0)) {
        *(usart->udr) = data;
        return 1;
    }
    return 0;
}


void
rfs_usart_close(struct rfs_usart *usart)
{
    *(usart->ucsrb) = 0;
}
