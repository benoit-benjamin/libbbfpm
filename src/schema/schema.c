/*
 * schema.c - functions for schema validation
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

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "logging/logging.h"
#include "schema/schema.h"
#include "schema/types.h"

bool _schema__validate( _parser__file__parsed* file__parsed )
{
    for ( uint16_t index__schema = 0; _bbfpm__schema[index__schema].name != NULL; index__schema++ )
    {
        bool found_coincidence = false;

        for ( uint64_t index__key = 0; index__key < file__parsed->keys__total; index__key++ )
        {
            if ( strcmp( file__parsed->keys[index__key].name,
                         _bbfpm__schema[index__schema].name ) != 0 )
                continue;

            if ( file__parsed->keys[index__key].type != _bbfpm__schema[index__schema].type )
            {
                if ( file__parsed->keys[index__key].type != _BBFPM__KEY__TYPE__NONE ||
                     ( file__parsed->keys[index__key].type == _BBFPM__KEY__TYPE__NONE &&
                       !_bbfpm__schema[index__schema].none_allowed ) )
                    continue;
            }

            file__parsed->keys[index__key].offset = _bbfpm__schema[index__schema].offset;

            found_coincidence = true;
        }

        if ( !found_coincidence )
        {
            _LOGGING__PRINT__ERROR(
                "schema: couldn't find metadata key coincidence. Expected metadata: %s",
                _bbfpm__schema[index__schema].name );
            return false;
        }
    }

    return true;
}
