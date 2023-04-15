/*
rfsavr.h - Headers for the rfsavr library.

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

////////////////// ERRNO ////////////////////////

/**
 * Errno value.
 */
extern uint8_t rfs_errno;

/**
 * Possible error codes.
 */
enum rfs_errno_codes {
    RFS_EFRAME = 1,
    RFS_EOVERRUN,
    RFS_EPARITY
};

////////////////// USART ////////////////////////

/**
 * The most typical Baudrate values.
 */
enum rfs_usart_baudrate
{
    RFS_USART_B50 = 50,
    RFS_USART_B75 = 75,
    RFS_USART_B110 = 110,
    RFS_USART_B134 = 134,
    RFS_USART_B150 = 150,
    RFS_USART_B200 = 200,
    RFS_USART_B300 = 300,
    RFS_USART_B600 = 600,
    RFS_USART_B1200 = 1200,
    RFS_USART_B1800 = 1800,
    RFS_USART_B2400 = 2400,
    RFS_USART_B4800 = 4800,
    RFS_USART_B9600 = 9600,
    RFS_USART_B19200 = 19200,
    RFS_USART_B38400 = 38400,
    RFS_USART_B57600 = 57600,
    RFS_USART_B115200 = 115200,
    RFS_USART_B230400 = 230400,
    RFS_USART_B460800 = 460800,
    RFS_USART_B500000 = 500000,
    RFS_USART_B576000 = 576000,
    RFS_USART_B921600 = 921600,
    RFS_USART_B1000000 = 1000000,
    RFS_USART_B1152000 = 1152000,
    RFS_USART_B1500000 = 1500000,
    RFS_USART_B2000000 = 2000000,
};

/**
 * Enumerates the available USART devices.
 */
enum rfs_usart_device
{
    RFS_USART_0,
};

/**
 * Enumetates the available USART mode.
 */
enum rfs_usart_mode
{
    RFS_USART_ASYNC = 0,
    RFS_USART_SYNCMASTER = 0x40,
    RFS_USART_SYNCSLAVE = 0x41,
    RFS_USART_MASTERSPI = 0xc0,
};

/**
 * Flags to setup the USART.
 */
enum rfs_usart_flags
{
    RFS_USART_MULTIPROCESSOR = 0x000001,
    RFS_USART_RX = 0x001000,
    RFS_USART_TX = 0x000800,
    RFS_USART_RXTX = 0x001800,
    RFS_USART_TXFALLING = 0x010000,
    RFS_USART_5BITS = 0x000000,
    RFS_USART_6BITS = 0x020000,
    RFS_USART_7BITS = 0x040000,
    RFS_USART_8BITS = 0x060000,
    RFS_USART_9BITS = 0x060400,
    RFS_USART_1STOPBIT = 0x000000,
    RFS_USART_2STOPBITS = 0x080000,
    RFS_USART_PARITYODD = 0x200000,
    RFS_USART_PARITYEVEN = 0x300000,
};

/**
 * Struct that contains all the information to operate the USART.
 */
struct rfs_usart
{
    volatile uint8_t *udr;
    volatile uint8_t *ucsra;
    volatile uint8_t *ucsrb;
    volatile uint8_t *ucsrc;
    volatile uint16_t *ubrr;
};

/**
 * Initializes the USART.
 *
 * This routine initializes the USART for its use.
 * A microcontroller can have more than one USART, so the USART to use is given in the device parameter.
 * @see rfs_usart_device
 * Then the user can choose a mode of operation, that can be Asynchronous, Synchronous Master,
 * Synchronous Slave or Master SPI.
 * @see rfs_usart_mode
 * Finally, flags contains the configuration options for each mode.
 * @see rfs_usart_flags
 *
 * @param usart Struct that represents the USART device. Contains the addresses to the registers to use for this device.
 * @param device The identifier to the device to use.
 * @param mode Mode of USART operation (Asynchronous, Synchronous Master, Synchronous Slave or Master SPI).
 * @param flags Flags to configure the USART.
 */
void
rfs_usart_open (struct rfs_usart *usart, enum rfs_usart_device device, enum rfs_usart_mode mode, int32_t flags);


/**
 * Set the speed for the USART communication.
 *
 * To correctly compute the parameters to configure the desired baudrate, the CPU frequency must
 * be known.
 * 
 * @param usart The usart to use.
 * @param baudrate The desired baudrate.
 * @param cpu_frequency The CPU frequency.
 */
void
rfs_usart_setspeed (struct rfs_usart *usart, enum rfs_usart_baudrate baudrate, uint32_t cpu_frequency);


/**
 * Read the next available character received by the USART.
 *
 * @param usart The usart to use.
 * @param data At output, contains the read character.
 * 
 * @returns 1 if a byte have been received without errors, 0 if there's no data or -1 on error. In this last case,
 *          rfs_errno contains the error, which can be:
 *          - RFS_EFRAME: Frame error, the stop bit was zero.
 *          - RFS_EOVERRUN: Data overrun, the input buffer is full and a start bit is detected.
 *          - RFS_EPARITY: Parity checking is enabled and the received bit has a parity error.
 */
int8_t
rfs_usart_read (struct rfs_usart *usart, char *data);


/**
 * Writes one byte to the USART.
 *
 * @param usart The usart to use.
 * @param data The byte to write.
 * 
 * @returns 1 if the byte have been written, 0 otherwise.
 */
int8_t
rfs_usart_write (struct rfs_usart *usart, char data);


/**
 * Disables the USART.
 *
 * @param usart The usart to disable.
 * 
 */
void
rfs_usart_close(struct rfs_usart *usart);