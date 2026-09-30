/*
 * schema.h - functions and data structures for validating metadata key entries
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

#ifndef BBFPM__SCHEMA__H
#define BBFPM__SCHEMA__H

#include <stdbool.h>
#include <stddef.h>

#include "parser/parser.h"

typedef struct _bbfpm__field_schema
{
    const char*       name;
    _bbfpm__key__type type;
    bool              none_allowed;
    uint64_t          offset;
} _bbfpm__field_schema;

extern _bbfpm__field_schema _bbfpm__schema[];

bool _bbfpm__verify__file__parsed__through_schema( _bbfpm__file__parsed* file__parsed );

#endif
