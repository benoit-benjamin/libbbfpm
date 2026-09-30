/*
 * parser.h - functions and data structures for parsing tokenized files
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

#ifndef BBFPM__PARSER__H
#define BBFPM__PARSER__H

#include "lexer/lexer.h"
#include "output/types.h"

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

bool          _bbfpm__parser__is_at_end( _bbfpm__file__parsed* file__parsed );
_bbfpm__token _bbfpm__parser__peek( _bbfpm__file__parsed* file__parsed );
_bbfpm__token _bbfpm__parser__advance( _bbfpm__file__parsed* file__parsed );
_bbfpm__token _bbfpm__parser__peek_next( _bbfpm__file__parsed* file__parsed, uint64_t offset );
bool          _bbfpm__parser__match( _bbfpm__file__parsed* file__parsed, _bbfpm__token expected );

_bbfpm__return_status _bbfpm__parser__push_key( _bbfpm__file__parsed* file__parsed,
                                                _bbfpm__key           key );
_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__arbitrary( _bbfpm__file__parsed* file__parsed,
                                                 _bbfpm__key*          key );
_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__list_correlated_arbitrary( _bbfpm__file__parsed* file__parsed,
                                                                 _bbfpm__key*          key );
_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__none( _bbfpm__file__parsed* file__parsed, _bbfpm__key* key );
_bbfpm__key _bbfpm__parser__parse_key( _bbfpm__file__parsed* file__parsed );

_bbfpm__return_status _bbfpm__file__parsed__parse( _bbfpm__file__parsed* file__parsed,
                                                   const char*           file__path );


#endif
