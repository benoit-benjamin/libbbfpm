/*
 * types.h - data structuctures and functions for parsed files
 * copyright (c) 2026  benjamin benoit
 *
 * this program is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * gnu general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with this program.  if not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BBFPM__PARSER__TYPES__H
#define BBFPM__PARSER__TYPES__H

#include <stdint.h>

#include "lexer/types.h"

typedef enum _bbfpm__key__type
{
    _BBFPM__KEY__TYPE__ARBITRARY,
    _BBFPM__KEY__TYPE__LIST_CORRELATED_ARBITRARY,
    _BBFPM__KEY__TYPE__NONE,
    _BBFPM__KEY__TYPE__UNDEFINED,
} _bbfpm__key__type;

typedef struct _bbfpm__key
{
    char              name[MAX__TOKEN__SIZE];
    _bbfpm__key__type type;
    void*             value;
    uint32_t          value__size;
    uint64_t          offset;
} _bbfpm__key;

typedef struct _bbfpm__file__parsed
{
    _bbfpm__key*             keys;
    _bbfpm__file__tokenized* file__tokenized;
    uint64_t                 keys__capacity;
    uint64_t                 keys__total;
} _bbfpm__file__parsed;

_bbfpm__return_status _bbfpm__file__parsed__initialize( _bbfpm__file__parsed* file__parsed,
                                                        const char*           file__path );
_bbfpm__return_status _bbfpm__file__parsed__free( _bbfpm__file__parsed* file__parsed );

#endif
