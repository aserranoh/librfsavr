/*
errno.h - Error codes definitions

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

#ifndef RFS_ERRNO_H
#define RFS_ERRNO_H

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

#endif