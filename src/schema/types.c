/*
 * types.c - data structures for schema validation
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

#include <stddef.h>

#include "data_mapper/types.h"
#include "schema/types.h"

_schema__field _bbfpm__schema[] = {
    { "NAME", _BBFPM__KEY__TYPE__ARBITRARY, false, offsetof( _data_mapper__metadata, name ) },
    { "REPOSITORY", _BBFPM__KEY__TYPE__ARBITRARY, true,
     offsetof( _data_mapper__metadata, repository ) },
    { "VERSION", _BBFPM__KEY__TYPE__ARBITRARY, true, offsetof( _data_mapper__metadata, version ) },
    { "RELEASE-DATE", _BBFPM__KEY__TYPE__ARBITRARY, true,
     offsetof( _data_mapper__metadata, release_date ) },
    { "AUTHORS", _BBFPM__KEY__TYPE__LIST_CORRELATED_ARBITRARY, false,
     offsetof( _data_mapper__metadata, authors ) },
    { "DEPENDENCIES", _BBFPM__KEY__TYPE__LIST_CORRELATED_ARBITRARY, true,
     offsetof( _data_mapper__metadata, dependencies ) },
    { "LICENSE", _BBFPM__KEY__TYPE__ARBITRARY, true, offsetof( _data_mapper__metadata, license ) },
    { NULL, _BBFPM__KEY__TYPE__UNDEFINED, false, 0 }
};
