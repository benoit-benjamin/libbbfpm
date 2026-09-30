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

typedef enum _parser__key__type
{
    _BBFPM__KEY__TYPE__ARBITRARY,
    _BBFPM__KEY__TYPE__LIST_CORRELATED_ARBITRARY,
    _BBFPM__KEY__TYPE__NONE,
    _BBFPM__KEY__TYPE__UNDEFINED,
} _parser__key__type;

typedef struct _parser__key
{
    char               name[MAX__TOKEN__SIZE];
    _parser__key__type type;
    void*              value;
    uint32_t           value__size;
    uint64_t           offset;
} _parser__key;

typedef struct _parser__file__parsed
{
    _parser__key*            keys;
    _lexer__file__tokenized* file__tokenized;
    uint64_t                 keys__capacity;
    uint64_t                 keys__total;
} _parser__file__parsed;

_output__status _parser__file__parsed__initialize( _parser__file__parsed* file__parsed,
                                                   const char*            file__path );
_output__status _parser__file__parsed__free( _parser__file__parsed* file__parsed );

#endif
