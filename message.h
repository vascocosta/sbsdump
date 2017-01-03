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

#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct {
    char  *type;
    int   subtype;
    int   session_id;
    int   aircraft_id;
    char  *hex_id;
    int   flight_id;
    char  *date;
    char  *time;
    char  *log_date;
    char  *log_time;
    char  *callsign;
    int   altitude;
    int   ground_speed;
    int   track;
    float latitude;
    float longitude;
    int   vertical_rate;
    int   squawk;
    int   emergency;
    int   spi;
    int   ground;
} MESSAGE;

int read_message(int socket_fd, char *buffer);
void parse_message (MESSAGE *message, char *buffer);

#endif
