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

#include "message.h"

#define USAGE                                \
    "Usage: sbsdump [OPTION]... hostname\n"  \
    "Dump data in SBS format from a socket." \
    "\n"                                     \
    "  -h display this help and exit\n"      \
    "  -p set port (default 30003)\n"        \
    "  -r show messages in raw format\n"     \
    "  -u show only unique hex ids\n" 

void show_usage()
{
    printf(USAGE);
}

int cmpfunc (const void *a, const void *b)
{
   return (*(unsigned long int*)a - *(unsigned long int*)b);
}

int connect_server(const char *hostname, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int socket_fd;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        return 0;
    }
    server = gethostbyname(hostname);
    if (server == NULL) {
        return 0;
    }
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        return 0;
    }
    return socket_fd;
}

int main(int argc, char *argv[])
{
    char buffer[256];
    unsigned long int hex_id_dec;
    unsigned long int hex_ids[1000] = {0};
    int hex_ids_i = 0;
    char *hostname = NULL;
    MESSAGE *message;
    int option;
    int option_r = 0;
    int option_u = 0;
    int port = 30003;
    int *result;
    int socket_fd;

    while ((option = getopt(argc, argv, "hp:ru")) != -1) {
        switch (option) {
            case 'h':
                show_usage();
                return 0;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'r':
                option_r = 1;
                break;
            case 'u':
                option_u = 1;
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
    if (!(socket_fd = connect_server(hostname, port))) {
        perror("Problem connecting to server");
        return 1;
    }
    message = (MESSAGE *)malloc(sizeof(MESSAGE));
    while (1) {
        if (!read_message(socket_fd, buffer, sizeof(buffer))) {
            perror("Problem reading from the server");
            return 1;
        }
        if (option_r) {
            printf("%s", buffer);
        } else {
            memset(message, 0, sizeof(MESSAGE));
            parse_message(message, buffer);
            if (option_u) {
                hex_id_dec = strtoul(message->hex_id, NULL, 16);
                qsort(hex_ids, 1000, sizeof(unsigned long int), cmpfunc);
                result = bsearch(&hex_id_dec, hex_ids, 1000, sizeof(unsigned long int), cmpfunc);
                if (result == NULL && strcmp(message->callsign, "empty") != 0) {
                    hex_ids[hex_ids_i++] = hex_id_dec;
                    printf("Hex ID: %s "
                           "Callsign: %s\n",
                           message->hex_id,
                           message->callsign);
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
