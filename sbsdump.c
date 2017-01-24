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

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "log.h"
#include "lookup.h"
#include "macros.h"
#include "message.h"

#define USAGE                                                       \
    "Usage: sbsdump [OPTION]... hostname\n"                         \
    "Dump data in SBS format from a socket."                        \
    "\n"                                                            \
    "  -c show only aircrafts with a callsign in spotting mode\n"   \
    "  -h display this help and exit\n"                             \
    "  -l log aircrafts into a data base when in spotting mode\n"   \
    "  -p set port (default 30003)\n"                               \
    "  -r show messages in raw format\n"                            \
    "  -s show only new aircrafts excluding dups (spotting mode)\n"

void show_usage()
{
    printf(USAGE);
}

int compare_hex_id(const void *a, const void *b)
{
   return (*(unsigned long int*)a - *(unsigned long int*)b);
}

int connect_server(const char *hostname, const char *service)
{
    struct addrinfo hints;
    struct addrinfo *res;
    int socket_fd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(hostname, service, &hints, &res) != 0) {
        return false;
    }
    socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd < 0) {
        return false;
    }
    if (connect(socket_fd, res->ai_addr, res->ai_addrlen) < 0) {
        return false;
    }
    freeaddrinfo(res);
    return socket_fd;
}

bool new_aircraft(MESSAGE *message, bool callsign, unsigned long int *hex_ids)
{
    unsigned long int hex_id_dec;
    static int hex_ids_i = 0;
    int *result;

    if (hex_ids_i == MAX_HEX_IDS) {
        hex_ids_i = 0;
        memset(hex_ids, 0, 4 * MAX_HEX_IDS);
    }
    hex_id_dec = strtoul(message->hex_id, NULL, 16);
    qsort(hex_ids, MAX_HEX_IDS, sizeof(unsigned long int), compare_hex_id);
    result = bsearch(&hex_id_dec,
                     hex_ids, MAX_HEX_IDS,
                     sizeof(unsigned long int),
                     compare_hex_id);
    if (result == NULL) {
        if (callsign && strcmp(message->callsign, "n/a") == 0) {
            return false;
        }
        hex_ids[0] = hex_id_dec;
        hex_ids_i++;
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    char aircraft_info[50][256];
    char buffer[256];
    unsigned long int hex_ids[MAX_HEX_IDS] = {0};
    char *hostname = NULL;
    MESSAGE *message;
    int option;
    int option_c = false;
    int option_l = false;
    int option_r = false;
    int option_s = false;
    char *service = NULL;
    int socket_fd;

    while ((option = getopt(argc, argv, "chlp:rs")) != -1) {
        switch (option) {
            case 'c':
                option_c = true;
                break;
            case 'h':
                show_usage();
                return 0;
                break;
            case 'l':
                option_l = true;
                break;
            case 'p':
                service = optarg;
                break;
            case 'r':
                option_r = true;
                break;
            case 's':
                option_s = true;
                break;
            default:
                show_usage();
                return 0;
        }
    }
    if ((hostname = argv[optind]) == NULL) {
        show_usage();
        return 0;
    }
    if (service == NULL) {
        service = "30003";
    }
    if (!(socket_fd = connect_server(hostname, service))) {
        perror("Problem connecting to server");
        return 1;
    }
    message = (MESSAGE *)malloc(sizeof(MESSAGE));
    while (true) {
        if (!read_message(socket_fd, buffer, sizeof(buffer))) {
            perror("Problem reading from the server");
            return 1;
        }
        if (option_r) {
            printf("%s", buffer);
        } else {
            memset(message, 0, sizeof(MESSAGE));
            parse_message(message, buffer);
            if (option_s) {
                if (new_aircraft(message, option_c, hex_ids)) {
                    lookup_aircraft(message->hex_id, aircraft_info);
                    printf("Date:\t\t%s\n"
                           "Time:\t\t%s\n"
                           "Hex:\t\t%s\n"
                           "Registration:\t%s\n"
                           "Model:\t\t%s\n"
                           "Callsign:\t%s\n"
                           "Airline:\t%s\n"
                           "Country:\t%s\n"
                           "FR24:\t\thttps://www.flightradar24.com/data/aircraft/%s\n\n",
                           message->date,
                           message->time,
                           message->hex_id,
                           aircraft_info[6],
                           aircraft_info[14],
                           message->callsign,
                           aircraft_info[21],
                           aircraft_info[4],
                           aircraft_info[6]);
                    if (option_l) {
                        if (!log_aircraft(message, aircraft_info)) {
                            perror("Problem logging aircraft");
                        }
                    }
                }
            } else {
                printf("Callsign: %s\n"
                       "Altitude: %d "
                       "Ground speed: %d "
                       "Vertical rate: %d\n"
                       "Track: %d "
                       "Lat: %f "
                       "Lon: %f\n"
                       "Squawk: %d\n\n",
                       message->callsign,
                       message->altitude,
                       message->ground_speed,
                       message->vertical_rate,
                       message->track,
                       message->latitude,
                       message->longitude,
                       message->squawk);
            }
        }
        fflush(stdout);
    }
    free(message);
    return 0;
}
