/*
 * parser.c - functions and data structures for parsing tokenized files
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
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "logging.h"
#include "parser.h"
#include "types.h"

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

bool _bbfpm__parser__is_at_end( _bbfpm__file__parsed* file__parsed )
{
    if ( file__parsed->file__tokenized->tokens__current_token__position >=
         file__parsed->file__tokenized->tokens__total )
    {
        return true;
    }
    return false;
}

_bbfpm__token _bbfpm__parser__peek( _bbfpm__file__parsed* file__parsed )
{
    return file__parsed->file__tokenized
        ->tokens[file__parsed->file__tokenized->tokens__current_token__position];
}

_bbfpm__token _bbfpm__parser__advance( _bbfpm__file__parsed* file__parsed )
{
    if ( !_bbfpm__parser__is_at_end( file__parsed ) )
    {
        return file__parsed->file__tokenized
            ->tokens[file__parsed->file__tokenized->tokens__current_token__position++];
    }
    return (_bbfpm__token) {
        .type = _BBFPM__TOKEN__TYPE__UNDEFINED, .column = 0, .line = 0, .value = { 0 }
    };
}

_bbfpm__token _bbfpm__parser__peek_next( _bbfpm__file__parsed* file__parsed, uint64_t offset )
{
    if ( file__parsed->file__tokenized->tokens__total >
         file__parsed->file__tokenized->tokens__current_token__position + offset )
    {
        return file__parsed->file__tokenized
            ->tokens[file__parsed->file__tokenized->tokens__current_token__position + offset];
    }
    return (_bbfpm__token) {
        .type = _BBFPM__TOKEN__TYPE__UNDEFINED, .column = 0, .line = 0, .value = { 0 }
    };
}

bool _bbfpm__parser__match( _bbfpm__file__parsed* file__parsed, _bbfpm__token expected )
{
    if ( file__parsed->file__tokenized
             ->tokens[file__parsed->file__tokenized->tokens__current_token__position]
             .type == expected.type )
    {
        ( !_bbfpm__parser__is_at_end( file__parsed ) )
            ? ++file__parsed->file__tokenized->tokens__current_token__position
            : file__parsed->file__tokenized->tokens__current_token__position;
        return true;
    }
    return false;
}

_bbfpm__return_status _bbfpm__parser__push_key( _bbfpm__file__parsed* file__parsed,
                                                _bbfpm__key           key )
{
    if ( file__parsed->keys__capacity / 2 <= file__parsed->keys__total )
    {
        if ( ( file__parsed->keys =
                   reallocarray( file__parsed->keys, file__parsed->keys__capacity * 2,
                                 sizeof( _bbfpm__key ) ) ) == NULL )
        {
            _BBFPM__LOG__PRINT__FATAL( "parser: couldn't reallocate allocated memory on "
                                       "_bbfpm__file__parsed struct member: 'keys'." );
            return _BBFPM__RETURN_STATUS__FAILURE;
        }
    }
    file__parsed->keys__capacity *= 2;
    file__parsed->keys[file__parsed->keys__total++] = key;
    return _BBFPM__RETURN_STATUS__SUCCESS;
}

_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__arbitrary( _bbfpm__file__parsed* file__parsed,
                                                 _bbfpm__key*          key )
{
    _bbfpm__token current_token = _bbfpm__parser__advance( file__parsed );

    key->type        = _BBFPM__KEY__TYPE__ARBITRARY;
    key->value__size = sizeof( current_token.value );
    key->value       = malloc( key->value__size );

    if ( key->value == NULL )
    {
        _BBFPM__LOG__PRINT__FATAL(
            "parser: failed allocating memory for _bbfpm__key struct member: 'value'." );
        return _BBFPM__RETURN_STATUS__FAILURE;
    }

    memset( key->value, 0, key->value__size );
    strncpy( key->value, current_token.value, key->value__size - 1 );

    while ( ( current_token = _bbfpm__parser__peek( file__parsed ) ).type !=
                _BBFPM__TOKEN__TYPE__CHARACTER__LINE_FEED &&
            !_bbfpm__parser__is_at_end( file__parsed ) )
        _bbfpm__parser__advance( file__parsed );

    _bbfpm__parser__advance( file__parsed );

    return _BBFPM__RETURN_STATUS__SUCCESS;
}
_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__list_correlated_arbitrary( _bbfpm__file__parsed* file__parsed,
                                                                 _bbfpm__key*          key )
{
    _bbfpm__token current_token          = _bbfpm__parser__peek( file__parsed );
    uint32_t      at_the_rate_sign_count = 0;
    uint64_t      loop_index             = 0;

    if ( current_token.type == _BBFPM__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE )
    {
        _BBFPM__LOG__PRINT__ERROR(
            "empty correlated arbitrary value list detected. Use NONE keyword instead" );
        return _BBFPM__RETURN_STATUS__FAILURE;
    }

    while ( ( current_token = _bbfpm__parser__peek_next( file__parsed, loop_index ) ).type !=
            _BBFPM__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE )
    {
        if ( current_token.type == _BBFPM__TOKEN__TYPE__CHARACTER__AT_THE_RATE_SIGN )
            at_the_rate_sign_count++;
        loop_index++;
    }

    current_token = (_bbfpm__token) { 0 };

    key->type        = _BBFPM__KEY__TYPE__LIST_CORRELATED_ARBITRARY;
    key->value__size = at_the_rate_sign_count * 2;
    key->value       = (char**) calloc( at_the_rate_sign_count * 2, sizeof( char* ) );
    for ( uint64_t i = 0; i < key->value__size; i++ )
    {
        ( (char**) key->value )[i] = malloc( MAX__TOKEN__SIZE );
        memset( ( (char**) key->value )[i], 0, MAX__TOKEN__SIZE );
    }
    uint64_t value__total = 0;

    if ( key->value == NULL )
    {
        _BBFPM__LOG__PRINT__FATAL(
            "parser: failed allocating memory for _bbfpm__key struct member: 'value'." );
        return _BBFPM__RETURN_STATUS__FAILURE;
    }

    while ( current_token.type != _BBFPM__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE )
    {
        current_token = _bbfpm__parser__advance( file__parsed );
        switch ( current_token.type )
        {
            case _BBFPM__TOKEN__TYPE__CHARACTER__QUOTATION_MARK:
                current_token = _bbfpm__parser__advance( file__parsed );
                strncpy( ( (char**) key->value )[value__total], current_token.value,
                         MAX__TOKEN__SIZE );
                value__total++;
                _bbfpm__parser__advance( file__parsed );
                continue;
            case _BBFPM__TOKEN__TYPE__CHARACTER__AT_THE_RATE_SIGN:
                current_token = _bbfpm__parser__advance( file__parsed );
                if ( current_token.type != _BBFPM__TOKEN__TYPE__CHARACTER__QUOTATION_MARK )
                {
                    _BBFPM__LOG__PRINT__ERROR(
                        "expecting arbitrary value correlation on token: line: %d column: %d",
                        current_token.line, current_token.column );
                    return _BBFPM__RETURN_STATUS__FAILURE;
                }
                current_token = _bbfpm__parser__peek( file__parsed );
                strncpy( ( (char**) key->value )[value__total], current_token.value,
                         MAX__TOKEN__SIZE );
                value__total++;
                _bbfpm__parser__advance( file__parsed );
                _bbfpm__parser__advance( file__parsed );
                continue;
            default:
                continue;
        }
    }

    while ( ( current_token = _bbfpm__parser__peek( file__parsed ) ).type !=
                _BBFPM__TOKEN__TYPE__CHARACTER__LINE_FEED &&
            !_bbfpm__parser__is_at_end( file__parsed ) )
        _bbfpm__parser__advance( file__parsed );

    _bbfpm__parser__advance( file__parsed );

    return _BBFPM__RETURN_STATUS__SUCCESS;
}
_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__none( _bbfpm__file__parsed* file__parsed, _bbfpm__key* key )
{
    _bbfpm__token current_token = { 0 };

    key->value       = NULL;
    key->type        = _BBFPM__KEY__TYPE__NONE;
    key->value__size = 0;

    while ( ( current_token = _bbfpm__parser__peek( file__parsed ) ).type !=
                _BBFPM__TOKEN__TYPE__CHARACTER__LINE_FEED &&
            !_bbfpm__parser__is_at_end( file__parsed ) )
        _bbfpm__parser__advance( file__parsed );

    return _BBFPM__RETURN_STATUS__SUCCESS;
}

_bbfpm__key _bbfpm__parser__parse_key( _bbfpm__file__parsed* file__parsed )
{
    _bbfpm__key key = { 0 };

    _bbfpm__token current_token = _bbfpm__parser__advance( file__parsed );

    if ( current_token.type == _BBFPM__TOKEN__TYPE__CHARACTER__EOF )
        goto on_error;

    while ( current_token.type == _BBFPM__TOKEN__TYPE__CHARACTER__HASH ||
            current_token.type == _BBFPM__TOKEN__TYPE__CHARACTER__LINE_FEED )
        current_token = _bbfpm__parser__advance( file__parsed );

    if ( current_token.type > _BBFPM__TOKEN__TYPE__KEYWORDS_LIMIT || current_token.type == -1 )
    {
        _BBFPM__LOG__PRINT__ERROR(
            "parser: undefined metadata key. token: line: %d column: %d type: %d",
            current_token.line, current_token.column, current_token.type );
        goto on_error;
    }

    strncpy( key.name, _bbfpm__token__type__keywords[current_token.type], MAX__TOKEN__SIZE - 1 );

    current_token = _bbfpm__parser__advance( file__parsed );
    if ( current_token.type != _BBFPM__TOKEN__TYPE__CHARACTER__COLON )
    {
        _BBFPM__LOG__PRINT__ERROR( "parser: expected token: ':' at token: line: %d column: %d",
                                   current_token.line, current_token.column );
        goto on_error;
    }

    current_token = _bbfpm__parser__advance( file__parsed );
    switch ( current_token.type )
    {
        case _BBFPM__TOKEN__TYPE__CHARACTER__QUOTATION_MARK:
            if ( _bbfpm__parser__parse_key__key__type__arbitrary( file__parsed, &key ) ==
                 _BBFPM__RETURN_STATUS__FAILURE )
                goto on_error;
            break;
        case _BBFPM__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__OPEN:
            if ( _bbfpm__parser__parse_key__key__type__list_correlated_arbitrary(
                     file__parsed, &key ) == _BBFPM__RETURN_STATUS__FAILURE )
                goto on_error;
            break;
        case _BBFPM__TOKEN__TYPE__KEYWORD__NONE:
            if ( _bbfpm__parser__parse_key__key__type__none( file__parsed, &key ) ==
                 _BBFPM__RETURN_STATUS__FAILURE )
                goto on_error;
            break;
        default:
            _BBFPM__LOG__PRINT__ERROR(
                "parser: expected key type arbitrary, list correlated arbitrary or "
                "none, token: line: %d column: %d",
                current_token.line, current_token.column );
            goto on_error;
    }

    return key;

on_error:
    key.type = _BBFPM__KEY__TYPE__UNDEFINED;
    return key;
}

_bbfpm__return_status _bbfpm__file__parsed__parse( _bbfpm__file__parsed* file__parsed,
                                                   const char*           file__path )
{
    if ( _bbfpm__file__parsed__initialize( file__parsed, file__path ) ==
         _BBFPM__RETURN_STATUS__FAILURE )
        return _BBFPM__RETURN_STATUS__FAILURE;

    while ( !_bbfpm__parser__is_at_end( file__parsed ) )
    {
        _bbfpm__key current_key = _bbfpm__parser__parse_key( file__parsed );
        if ( current_key.type == _BBFPM__KEY__TYPE__UNDEFINED &&
             _bbfpm__parser__peek( file__parsed ).type != _BBFPM__TOKEN__TYPE__CHARACTER__EOF )
            return _BBFPM__RETURN_STATUS__FAILURE;
        if ( _bbfpm__parser__push_key( file__parsed, current_key ) ==
             _BBFPM__RETURN_STATUS__FAILURE )
            return _BBFPM__RETURN_STATUS__FAILURE;
        if ( _bbfpm__parser__peek( file__parsed ).type == _BBFPM__TOKEN__TYPE__CHARACTER__EOF )
            break;
    }
    return _BBFPM__RETURN_STATUS__SUCCESS;
}
