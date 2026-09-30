/*
 * types.c - data structures and functions for parsed files
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

#include <stddef.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "logging/logging.h"
#include "parser/types.h"

_bbfpm__return_status _bbfpm__file__parsed__initialize( _bbfpm__file__parsed* file__parsed,
                                                        const char*           file__path )
{
    _bbfpm__file__tokenized* file__tokenized = malloc( sizeof( _bbfpm__file__tokenized ) );
    if ( _bbfpm__file__tokenized__initialize( file__tokenized, file__path ) ==
         _BBFPM__RETURN_STATUS__FAILURE )
        return _BBFPM__RETURN_STATUS__FAILURE;

    if ( _bbfpm__file__tokenize( file__tokenized ) == _BBFPM__RETURN_STATUS__FAILURE )
        return _BBFPM__RETURN_STATUS__FAILURE;

    file__parsed->file__tokenized = file__tokenized;

    static const uint64_t KEYS__CAPACITY__INITIAL_VALUE = 10;
    file__parsed->keys__capacity                        = KEYS__CAPACITY__INITIAL_VALUE;
    file__parsed->keys__total                           = 0;
    if ( ( file__parsed->keys = calloc( KEYS__CAPACITY__INITIAL_VALUE, sizeof( _bbfpm__key ) ) ) ==
         NULL )
    {
        _BBFPM__LOG__PRINT__FATAL(
            "parser: failed allocating memory for _bbfpm__file__parsed struct member: 'keys'." );
        return _BBFPM__RETURN_STATUS__FAILURE;
    }

    return _BBFPM__RETURN_STATUS__SUCCESS;
}

_bbfpm__return_status _bbfpm__file__parsed__free( _bbfpm__file__parsed* file__parsed )
{
    _bbfpm__file__tokenized__free( file__parsed->file__tokenized );

    for ( uint64_t index__key = 0; index__key < file__parsed->keys__total; index__key++ )
    {
        switch ( file__parsed->keys[index__key].type )
        {
            case _BBFPM__KEY__TYPE__ARBITRARY:
                free( (char*) ( file__parsed->keys[index__key].value ) );
                continue;
            case _BBFPM__KEY__TYPE__LIST_CORRELATED_ARBITRARY:
                for ( uint32_t value__element = 0;
                      value__element < file__parsed->keys[index__key].value__size;
                      value__element++ )
                    free( ( (char**) file__parsed->keys[index__key].value )[value__element] );
                free( ( (char**) file__parsed->keys[index__key].value ) );
                continue;
            case _BBFPM__KEY__TYPE__NONE:
                continue;
            case _BBFPM__KEY__TYPE__UNDEFINED:
                continue;
        }
    }

    free( file__parsed->keys );
    file__parsed->keys           = NULL;
    file__parsed->keys__capacity = 0;
    file__parsed->keys__total    = 0;
    return _BBFPM__RETURN_STATUS__SUCCESS;
}
