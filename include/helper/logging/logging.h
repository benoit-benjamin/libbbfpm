/*
 * logging.h - internal logging functions
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

#ifndef BBFPM__LOGGING__H
#define BBFPM__LOGGING__H

#define MAX__LOG__MESSAGE__SIZE 2048

typedef enum _logging__level
{
    // Skipped trace logging level because of code
    // legibility.
    _LOGGING__LEVEL__DEBUG = 0,
    _LOGGING__LEVEL__INFO  = 1,
    _LOGGING__LEVEL__WARN  = 2,
    _LOGGING__LEVEL__ERROR = 3,
    _LOGGING__LEVEL__FATAL = 4,
} _logging__level;

void _logging__print( _logging__level log__level, const char* file__path, const char* function,
                      int line, const char* format, ... );

#define _LOGGING__PRINT__DEBUG( ... ) \
    _logging__print( _LOGGING__LEVEL__DEBUG, __FILE__, __func__, __LINE__, ##__VA_ARGS__ )

#define _LOGGING__PRINT__INFO( ... ) \
    _logging__print( _LOGGING__LEVEL__INFO, __FILE__, __func__, __LINE__, ##__VA_ARGS__ )

#define _LOGGING__PRINT__WARN( ... ) \
    _logging__print( _LOGGING__LEVEL__WARN, __FILE__, __func__, __LINE__, ##__VA_ARGS__ )

#define _LOGGING__PRINT__ERROR( ... ) \
    _logging__print( _LOGGING__LEVEL__ERROR, __FILE__, __func__, __LINE__, ##__VA_ARGS__ )

#define _LOGGING__PRINT__FATAL( ... ) \
    _logging__print( _LOGGING__LEVEL__FATAL, __FILE__, __func__, __LINE__, ##__VA_ARGS__ )


#endif
