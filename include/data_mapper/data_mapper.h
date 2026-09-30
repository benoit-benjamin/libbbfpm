/*
 * data_mapper.h - data mapping functions and data structures for parsed and verified data
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

#ifndef BBFPM__DATA_MAPPER__H
#define BBFPM__DATA_MAPPER__H

#include "output/types.h"
#include "parser/parser.h"

typedef struct _bbfpm__data__entry
{
    void*    value;
    uint32_t extent;
} _bbfpm__data__entry;

typedef struct _bbfpm__data
{
    _bbfpm__data__entry name;
    _bbfpm__data__entry repository;
    _bbfpm__data__entry version;
    _bbfpm__data__entry release_date;
    _bbfpm__data__entry license;
    _bbfpm__data__entry authors;
    _bbfpm__data__entry dependencies;
} _bbfpm__mapped_metadata;

_bbfpm__return_status
_bbfpm__map_metadata_from__file__parsed( _bbfpm__file__parsed*    file__parsed,
                                         _bbfpm__mapped_metadata* mapped_metadata );

_bbfpm__return_status _bbfpm__mapped_metadata__free( _bbfpm__mapped_metadata* mapped_metadata,
                                                     _bbfpm__file__parsed*    file__parsed );

#endif
