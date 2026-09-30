/*
 * parser.c - functions for parsing tokenized files
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

#include "lexer/lexer.h"
#include "lexer/types.h"
#include "logging/logging.h"
#include "output/types.h"
#include "parser/parser.h"
#include "parser/primitives.h"
#include "parser/types.h"

_output__status _parser__key__push( _parser__file__parsed* file__parsed, _parser__key key )
{
    if ( file__parsed->keys__capacity / 2 <= file__parsed->keys__total )
    {
        if ( ( file__parsed->keys =
                   reallocarray( file__parsed->keys, file__parsed->keys__capacity * 2,
                                 sizeof( _parser__key ) ) ) == NULL )
        {
            _LOGGING__PRINT__FATAL( "parser: couldn't reallocate allocated memory on "
                                    "_parser__file__parsed struct member: 'keys'." );
            return _OUTPUT__STATUS__FAILURE;
        }
    }
    file__parsed->keys__capacity *= 2;
    file__parsed->keys[file__parsed->keys__total++] = key;
    return _OUTPUT__STATUS__SUCCESS;
}

_output__status _parser__key__parse__arbitrary( _parser__file__parsed* file__parsed,
                                                _parser__key*          key )
{
    _lexer__token current_token = _parser__advance( file__parsed );

    key->type        = _BBFPM__KEY__TYPE__ARBITRARY;
    key->value__size = sizeof( current_token.value );
    key->value       = malloc( key->value__size );

    if ( key->value == NULL )
    {
        _LOGGING__PRINT__FATAL(
            "parser: failed allocating memory for _parser__key struct member: 'value'." );
        return _OUTPUT__STATUS__FAILURE;
    }

    memset( key->value, 0, key->value__size );
    strncpy( key->value, current_token.value, key->value__size - 1 );

    while ( ( current_token = _parser__peek( file__parsed ) ).type !=
                _LEXER__TOKEN__TYPE__CHARACTER__LINE_FEED &&
            !_parser__is_at_end( file__parsed ) )
        _parser__advance( file__parsed );

    _parser__advance( file__parsed );

    return _OUTPUT__STATUS__SUCCESS;
}
_output__status
_parser__key__parse__list_correrlated_arbitrary( _parser__file__parsed* file__parsed,
                                                 _parser__key*          key )
{
    _lexer__token current_token          = _parser__peek( file__parsed );
    uint32_t      at_the_rate_sign_count = 0;
    uint64_t      loop_index             = 0;

    if ( current_token.type == _LEXER__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE )
    {
        _LOGGING__PRINT__ERROR(
            "empty correlated arbitrary value list detected. Use NONE keyword instead" );
        return _OUTPUT__STATUS__FAILURE;
    }

    while ( ( current_token = _parser__peek_next( file__parsed, loop_index ) ).type !=
            _LEXER__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE )
    {
        if ( current_token.type == _LEXER__TOKEN__TYPE__CHARACTER__AT_THE_RATE_SIGN )
            at_the_rate_sign_count++;
        loop_index++;
    }

    current_token = (_lexer__token) { 0 };

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
        _LOGGING__PRINT__FATAL(
            "parser: failed allocating memory for _parser__key struct member: 'value'." );
        return _OUTPUT__STATUS__FAILURE;
    }

    while ( current_token.type != _LEXER__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE )
    {
        current_token = _parser__advance( file__parsed );
        switch ( current_token.type )
        {
            case _LEXER__TOKEN__TYPE__CHARACTER__QUOTATION_MARK:
                current_token = _parser__advance( file__parsed );
                strncpy( ( (char**) key->value )[value__total], current_token.value,
                         MAX__TOKEN__SIZE );
                value__total++;
                _parser__advance( file__parsed );
                continue;
            case _LEXER__TOKEN__TYPE__CHARACTER__AT_THE_RATE_SIGN:
                current_token = _parser__advance( file__parsed );
                if ( current_token.type != _LEXER__TOKEN__TYPE__CHARACTER__QUOTATION_MARK )
                {
                    _LOGGING__PRINT__ERROR(
                        "expecting arbitrary value correlation on token: line: %d column: %d",
                        current_token.line, current_token.column );
                    return _OUTPUT__STATUS__FAILURE;
                }
                current_token = _parser__peek( file__parsed );
                strncpy( ( (char**) key->value )[value__total], current_token.value,
                         MAX__TOKEN__SIZE );
                value__total++;
                _parser__advance( file__parsed );
                _parser__advance( file__parsed );
                continue;
            default:
                continue;
        }
    }

    while ( ( current_token = _parser__peek( file__parsed ) ).type !=
                _LEXER__TOKEN__TYPE__CHARACTER__LINE_FEED &&
            !_parser__is_at_end( file__parsed ) )
        _parser__advance( file__parsed );

    _parser__advance( file__parsed );

    return _OUTPUT__STATUS__SUCCESS;
}
_output__status _parser__key__parse__none( _parser__file__parsed* file__parsed, _parser__key* key )
{
    _lexer__token current_token = { 0 };

    key->value       = NULL;
    key->type        = _BBFPM__KEY__TYPE__NONE;
    key->value__size = 0;

    while ( ( current_token = _parser__peek( file__parsed ) ).type !=
                _LEXER__TOKEN__TYPE__CHARACTER__LINE_FEED &&
            !_parser__is_at_end( file__parsed ) )
        _parser__advance( file__parsed );

    _parser__advance( file__parsed );

    return _OUTPUT__STATUS__SUCCESS;
}

_parser__key _parser__key__parse( _parser__file__parsed* file__parsed )
{
    _parser__key key = { 0 };

    _lexer__token current_token = _parser__advance( file__parsed );

    if ( current_token.type == _LEXER__TOKEN__TYPE__CHARACTER__EOF )
        goto on_error;

    while ( current_token.type == _LEXER__TOKEN__TYPE__CHARACTER__HASH ||
            current_token.type == _LEXER__TOKEN__TYPE__CHARACTER__LINE_FEED )
        current_token = _parser__advance( file__parsed );

    if ( current_token.type > _LEXER__TOKEN__TYPE__KEYWORDS_LIMIT || current_token.type == -1 )
    {
        _LOGGING__PRINT__ERROR(
            "parser: undefined metadata key. token: line: %d column: %d type: %d",
            current_token.line, current_token.column, current_token.type );
        goto on_error;
    }

    strncpy( key.name, _lexer__keywords[current_token.type], MAX__TOKEN__SIZE - 1 );

    current_token = _parser__advance( file__parsed );
    if ( current_token.type != _LEXER__TOKEN__TYPE__CHARACTER__COLON )
    {
        _LOGGING__PRINT__ERROR( "parser: expected token: ':' at token: line: %d column: %d",
                                current_token.line, current_token.column );
        goto on_error;
    }

    current_token = _parser__advance( file__parsed );
    switch ( current_token.type )
    {
        case _LEXER__TOKEN__TYPE__CHARACTER__QUOTATION_MARK:
            if ( _parser__key__parse__arbitrary( file__parsed, &key ) == _OUTPUT__STATUS__FAILURE )
                goto on_error;
            break;
        case _LEXER__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__OPEN:
            if ( _parser__key__parse__list_correrlated_arbitrary( file__parsed, &key ) ==
                 _OUTPUT__STATUS__FAILURE )
                goto on_error;
            break;
        case _LEXER__TOKEN__TYPE__KEYWORD__NONE:
            if ( _parser__key__parse__none( file__parsed, &key ) == _OUTPUT__STATUS__FAILURE )
                goto on_error;
            break;
        default:
            _LOGGING__PRINT__ERROR(
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

_output__status _parser__file__parsed__parse( _parser__file__parsed* file__parsed,
                                              const char*            file__path )
{
    if ( _parser__file__parsed__initialize( file__parsed, file__path ) == _OUTPUT__STATUS__FAILURE )
        return _OUTPUT__STATUS__FAILURE;

    while ( !_parser__is_at_end( file__parsed ) )
    {
        _parser__key current_key = _parser__key__parse( file__parsed );
        if ( current_key.type == _BBFPM__KEY__TYPE__UNDEFINED &&
             _parser__peek( file__parsed ).type != _LEXER__TOKEN__TYPE__CHARACTER__EOF )
            return _OUTPUT__STATUS__FAILURE;
        if ( _parser__key__push( file__parsed, current_key ) == _OUTPUT__STATUS__FAILURE )
            return _OUTPUT__STATUS__FAILURE;
        if ( _parser__peek( file__parsed ).type == _LEXER__TOKEN__TYPE__CHARACTER__EOF )
            break;
    }
    return _OUTPUT__STATUS__SUCCESS;
}
