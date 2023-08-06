/*
message.h - Defines message protocol to be sent using the USART subsystem

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

#ifndef RFS_MESSAGE_H
#define RFS_MESSAGE_H

#include <stdint.h>

#include <rfsavr/usart.h>

/**
 * @brief Used internaly to implement te state machine that sends the message through the serial port
 */
enum rfs_message_state
{
    RFS_MSG_STATE_HEADER,
    RFS_MSG_STATE_DATA,
    RFS_MSG_STATE_CHECKSUM1,
    RFS_MSG_STATE_CHECKSUM2,
    RFS_MSG_STATE_END
};

/**
 * @brief Struct that contains all the necessary information to operate with the message.
 */
struct rfs_message_t
{
    const char *data;
    uint8_t size;
    struct rfs_usart_t *usart;
    int8_t checksum;
    uint8_t written;
    const char *data_ptr;
    enum rfs_message_state state;
};

/**
 * @brief Initializes the message
 * 
 * @param message The message to be sent
 * @param usart The USART devide used to send the message
 * @param data The data to be sent
 * @param size The size of the data to be sent
 */
void rfs_message_init(struct rfs_message_t *message, struct rfs_usart_t *usart, const char *data, uint8_t size);

/**
 * @brief Send a part of the message
 *
 * This function is non blocking. If the message has not been completely sent, then it returns
 * 1. Otherwise, it returns 0.
 * 
 * @param message The message to be sent
 * 
 * @returns Whether the message has been completely sent or not
 */
int8_t rfs_message_send(struct rfs_message_t *message);

#endif