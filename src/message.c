/*
message.c - Defines message protocol to be sent using the USART subsystem

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

#include <rfsavr/message.h>
#include <rfsavr/string.h>

//////////////////////////////////////////////////// CONSTANTS ///////////////////////////////////////////////////////

#define RFS_MSG_HEADER  ':'

///////////////////////////////////////////////// PRIVATE FUNCTIONS //////////////////////////////////////////////////


/**
 * @brief Compute the checksum of a given sequence of bytes
 * 
 * @param data The sequence of bytes
 * @param size Number of bytes
 * 
 * @returns The checksum of the given sequence of bytes
 */
static int8_t rfs_message_checksum(const char *data, uint8_t size)
{
    int8_t sum = 0;

    for (int8_t i = 0; i < size; i++) {
        sum = (sum + data[i]) & 0xff;
    }
    return ((sum ^ 0xff) + 1) & 0xff;
}


///////////////////////////////////////////////// PUBLIC FUNCTIONS ///////////////////////////////////////////////////


void rfs_message_init(struct rfs_message_t *message, struct rfs_usart_t *usart, const char *data, uint8_t size)
{
    message->data = data;
    message->size = size;
    message->usart = usart;
    message->written = 0;
    message->data_ptr = message->data;
    message->state = RFS_MSG_STATE_HEADER;
    message->checksum = rfs_message_checksum(message->data, message->size);
}

int8_t rfs_message_send(struct rfs_message_t *message)
{
    switch (message->state) {
    case RFS_MSG_STATE_HEADER:
        if (rfs_usart_write(message->usart, RFS_MSG_HEADER)) {
            message->state = RFS_MSG_STATE_DATA;
        }
        break;
    case RFS_MSG_STATE_DATA:
        if (rfs_usart_write(message->usart, *message->data_ptr)) {
            message->written++;
            message->data_ptr++;
        }
        if (message->written == message->size) {
            message->state = RFS_MSG_STATE_CHECKSUM1;
        }
        break;
    case RFS_MSG_STATE_CHECKSUM1:
        if (rfs_usart_write(message->usart, rfs_str_itohex((message->checksum >> 4) & 0x0f))) {
            message->state = RFS_MSG_STATE_CHECKSUM2;
        }
        break;
    case RFS_MSG_STATE_CHECKSUM2:
        if (rfs_usart_write(message->usart, rfs_str_itohex(message->checksum & 0x0f))) {
            message->state = RFS_MSG_STATE_END;
        }
        break;
    case RFS_MSG_STATE_END:
        if (rfs_usart_write(message->usart, '\n')) {
            return 0;
        }
        break;
    }
    return 1;
}
