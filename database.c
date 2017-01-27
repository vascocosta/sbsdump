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

#include <sqlite3.h>
#include <string.h>

#include "macros.h"
#include "message.h"

static int write_result(void *result_pointer,
                        int number_columns,
                        char **fields,
                        char **column_names)
{
    int i;
    char (*result)[256] = (char (*)[256])result_pointer;

    for(i = 0; i != number_columns; i++) {
        if (fields[i] != NULL) {
            strcpy(result[i], fields[i]);
        }
    }
    return 0;
}

bool log_aircraft(MESSAGE *message, char aircraft_info[][256], bool daily)
{
    sqlite3 *db;
    char result[50][256];
    char *sql;
    int sqlite_code;

    if (daily) {
        sqlite_code = sqlite3_open(LOG_DB, &db);
        if (sqlite_code) {
            sqlite3_close(db);
            return false;
        }
        sql = sqlite3_mprintf("SELECT * from aircrafts \
                               where hex_id = '%q' \
                               and date = '%q' \
                               COLLATE NOCASE",
                              message->hex_id,
                              message->date);
        sqlite_code = sqlite3_exec(db, sql, write_result, (void *)result, 0);
        sqlite3_free(sql);
        sqlite3_close(db);
        if (sqlite_code ||
           (result[3] != NULL && (strcmp(result[3], message->hex_id) == 0))) {
            return true;
        }
    }
    sqlite_code = sqlite3_open(LOG_DB, &db);
    if (sqlite_code) {
        sqlite3_close(db);
        return false;
    }
    sql = sqlite3_mprintf("INSERT INTO aircrafts VALUES("
                          "NULL, "
                          "'%q', "
                          "'%q', "
                          "'%q', "
                          "'%q', "
                          "'%q', "
                          "'%q', "
                          "'%q', "
                          "'%q', "
                          "'https://www.flightradar24.com/data/aircraft/%q')",
                          message->date,
                          message->time,
                          message->hex_id,
                          aircraft_info[6],
                          aircraft_info[14],
                          message->callsign,
                          aircraft_info[21],
                          aircraft_info[4],
                          aircraft_info[6]);
    sqlite_code = sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_free(sql);
    sqlite3_close(db);
    if (sqlite_code) {
        return false;
    }
    return true;
}

bool lookup_aircraft(const char *hex_id, char result[][256])
{
    sqlite3 *db;
    char *sql;
    int sqlite_code;

    sqlite_code = sqlite3_open(LOOKUP_DB, &db);
    if (sqlite_code) {
        sqlite3_close(db);
        return false;
    }
    sql = sqlite3_mprintf("SELECT * from Aircraft \
                           where ModeS = '%q' \
                           COLLATE NOCASE",
                          hex_id);
    sqlite_code = sqlite3_exec(db, sql, write_result, (void *)result, 0);
    sqlite3_free(sql);
    sqlite3_close(db);
    if (sqlite_code) {
        return false;
    }
    return true;
}
