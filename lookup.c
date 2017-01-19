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

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "lookup.h"
#include "macros.h"

static size_t write_response(void *data, size_t size, size_t elements, void *user_data)
{
    HTTP_RESPONSE *http_response = (HTTP_RESPONSE *)user_data;

    http_response->data = realloc(http_response->data, http_response->size + (size * elements) + 1);
    if(http_response->data == NULL) {
        printf("Not enough memory\n");
        return 0;
    }
    memcpy(&(http_response->data[http_response->size]), data, size * elements);
    http_response->size += (size * elements);
    http_response->data[http_response->size] = 0;
    return (size * elements);
}

char *lookup_aircraft(const char *info, const char *hex_id)
{
    CURLcode curl_code;
    CURL *curl_handle;
    HTTP_RESPONSE http_response; 
    http_response.data = malloc(1);
    http_response.size = 0;
    static char result[512] = "";
    char url[512] = "https://ae.roplan.es/api/hex-";

    strncat(url, info, 256);
    strncat(url, ".php?hex=", 10);
    strncat(url, hex_id, 32);
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&http_response);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_code = curl_easy_perform(curl_handle);
    if(curl_code != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(curl_code));
    }
    else {
        strcpy(result, http_response.data);
    }
    curl_easy_cleanup(curl_handle);
    free(http_response.data);
    return result;
}
