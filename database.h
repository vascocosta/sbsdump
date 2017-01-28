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

#ifndef DATABASE_H
#define DATABASE_H

#include "macros.h"
#include "message.h"

bool log_aircraft(MESSAGE *message, char aircraft_info[][256], bool daily);
bool lookup_aircraft(const char *hex_id, char result[][256], size_t n);

#endif
