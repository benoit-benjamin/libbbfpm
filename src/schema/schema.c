/*
 * schema.c - functions and data structures for validating metadata key entries
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

#include "data_mapper.h"
#include "logging.h"
#include "parser.h"
#include "schema.h"

_bbfpm__field_schema _bbfpm__schema[] = {
    { "NAME", _BBFPM__KEY__TYPE__ARBITRARY, false, offsetof( _bbfpm__mapped_metadata, name ) },
    { "REPOSITORY", _BBFPM__KEY__TYPE__ARBITRARY, true,
     offsetof( _bbfpm__mapped_metadata, repository ) },
    { "VERSION", _BBFPM__KEY__TYPE__ARBITRARY, true, offsetof( _bbfpm__mapped_metadata, version ) },
    { "RELEASE-DATE", _BBFPM__KEY__TYPE__ARBITRARY, true,
     offsetof( _bbfpm__mapped_metadata, release_date ) },
    { "AUTHORS", _BBFPM__KEY__TYPE__LIST_CORRELATED_ARBITRARY, false,
     offsetof( _bbfpm__mapped_metadata, authors ) },
    { "DEPENDENCIES", _BBFPM__KEY__TYPE__LIST_CORRELATED_ARBITRARY, true,
     offsetof( _bbfpm__mapped_metadata, dependencies ) },
    { "LICENSE", _BBFPM__KEY__TYPE__ARBITRARY, true, offsetof( _bbfpm__mapped_metadata, license ) },
    { NULL, _BBFPM__KEY__TYPE__UNDEFINED, false, 0 }
};

bool _bbfpm__verify__file__parsed__through_schema( _bbfpm__file__parsed* file__parsed )
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
            _BBFPM__LOG__PRINT__ERROR(
                "schema: couldn't find metadata key coincidence. Expected metadata: %s",
                _bbfpm__schema[index__schema].name );
            return false;
        }
    }

    return true;
}
