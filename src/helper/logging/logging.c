/*
 * logging.c - internal logging functions
 * Copyright (C) 2026  Benjamin Benoit
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "logging/logging.h"

void _bbfpm__log__print( _bbfpm__log__level log__level, const char* file__path,
                         const char* function, int line, const char* format, ... )
{
    static const char* LOG__LEVEL_PREFIX[] = { "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL" };

    char log__message[MAX__LOG__MESSAGE__SIZE] = { 0 };

    const char* file__name =
        ( ( file__name = strrchr( file__path, '/' ) ) == NULL ) ? file__path : file__name + 1;

    int log__message__length =
        snprintf( log__message, MAX__LOG__MESSAGE__SIZE,
                  "[LIBCBBFPM] [%s] [%s] [%s:%d]: ", LOG__LEVEL_PREFIX[log__level], file__name,
                  function, line );

    va_list variable_arguments_list = { 0 };
    va_start( variable_arguments_list, format );
    vsnprintf( log__message + log__message__length, MAX__LOG__MESSAGE__SIZE - log__message__length,
               format, variable_arguments_list );
    va_end( variable_arguments_list );
    fprintf( stderr, "%s\n", log__message );
}
