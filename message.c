/*
 * Copyright (C) 2017 Vasco Costa <vascomacosta at gmail dot com>
 *
 * This file is part of sbsdump.
 *
 * sbsdump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * sbsdump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with sbsdump.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "macros.h"
#include "message.h"

bool read_message(int socket_fd, char *buffer, size_t n)
{
    char ch[] = "";

    memset(buffer, 0, n);
    while (*ch != '\n' && n--) {
        if (!read(socket_fd, ch, 1))
            return false;
        strncat(buffer, ch, 1);
    }
    return true;
}

void parse_message(MESSAGE *message, char *buffer)
{
    char *fields[22];
    int i;
    char *pch;
    char *start;

    start = buffer;
    for (i = 0; i != 22; i++) {
        if (*start != ',') {
            pch = strchr(start, ',');
            if (pch != NULL) {
                *pch = '\0';
                fields[i] = start;
                start = pch + 1;
            }
        } else {
            fields[i] = "n/a";
            start += 1;
        }
    }
    message->type = fields[0];
    message->subtype = atoi(fields[1]);
    message->session_id = atoi(fields[2]);
    message->aircraft_id = atoi(fields[3]);
    message->hex_id = fields[4];
    message->flight_id = atoi(fields[5]);
    message->date = fields[6];
    message->time = fields[7];
    message->log_date = fields[8];
    message->log_time = fields[9];
    message->callsign = fields[10];
    message->altitude = atoi(fields[11]);
    message->ground_speed = atoi(fields[12]);
    message->track = atoi(fields[13]);
    message->latitude = atof(fields[14]);
    message->longitude = atof(fields[15]);
}
