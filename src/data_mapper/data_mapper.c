/*
 * data_mapper.c - data mapping functions and structures for parsed and verified data
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

#include "data_mapper.h"
#include "../parser/parser.h"

_bbfpm__return_status
_bbfpm__map_metadata_from__file__parsed( _bbfpm__file__parsed*    file__parsed,
                                         _bbfpm__mapped_metadata* mapped_metadata )
{
    for ( uint64_t index__keys = 0; index__keys < file__parsed->keys__total; index__keys++ )
    {
        _bbfpm__key          current_key = file__parsed->keys[index__keys];
        _bbfpm__data__entry* data_entry =
            (_bbfpm__data__entry*) ( (char*) mapped_metadata + current_key.offset );
        data_entry->value  = current_key.value;
        data_entry->extent = current_key.value__size;
    }

    return _BBFPM__RETURN_STATUS__SUCCESS;
}

_bbfpm__return_status _bbfpm__mapped_metadata__free( _bbfpm__mapped_metadata* mapped_metadata,
                                                     _bbfpm__file__parsed*    file__parsed )
{
    if ( _bbfpm__file__parsed__free( file__parsed ) == _BBFPM__RETURN_STATUS__FAILURE )
        return _BBFPM__RETURN_STATUS__FAILURE;

    mapped_metadata->name.value         = NULL;
    mapped_metadata->repository.value   = NULL;
    mapped_metadata->version.value      = NULL;
    mapped_metadata->release_date.value = NULL;
    mapped_metadata->authors.value      = NULL;
    mapped_metadata->dependencies.value = NULL;
    mapped_metadata->license.value      = NULL;

    return _BBFPM__RETURN_STATUS__SUCCESS;
}
