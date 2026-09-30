/*
 * lexer.c - functions for file tokenization
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "lexer/primitives.h"
#include "logging/logging.h"

_output__status _lexer__token__push( _lexer__file__tokenized* file__tokenized, _lexer__token token )
{
    if ( file__tokenized->tokens__capacity / 2 <= file__tokenized->tokens__total )
    {
        if ( ( file__tokenized->tokens =
                   reallocarray( file__tokenized->tokens, file__tokenized->tokens__capacity * 2,
                                 sizeof( _lexer__token ) ) ) == NULL )
        {
            _LOGGING__PRINT__FATAL( "lexer: couldn't reallocate allocated memory on "
                                    "_lexer__file__tokenized struct member: 'tokens'." );
            return _OUTPUT__STATUS__FAILURE;
        }
        file__tokenized->tokens__capacity *= 2;
    }
    file__tokenized->tokens[file__tokenized->tokens__total++] = token;
    return _OUTPUT__STATUS__SUCCESS;
}

_lexer__token _lexer__token__scan__character( _lexer__file__tokenized* file__tokenized,
                                              _lexer__token__type      type )
{
    _lexer__token token = {
        .type   = type,
        .value  = { 0 },
        .line   = file__tokenized->tokens__current_character__line,
        .column = file__tokenized->tokens__current_character__column,
    };

    if ( type == _LEXER__TOKEN__TYPE__CHARACTER__HASH )
    {
        while ( _lexer__peek( file__tokenized ) != '\n' && !_lexer__is_at_end( file__tokenized ) )
        {
            _lexer__advance( file__tokenized );
        }
    }

    return token;
}

_lexer__token _lexer__token__scan__keyword( _lexer__file__tokenized* file__tokenized )
{
    _lexer__token token = {
        .type   = _LEXER__TOKEN__TYPE__UNDEFINED,
        .value  = { 0 },
        .line   = file__tokenized->tokens__current_character__line,
        .column = file__tokenized->tokens__current_character__column,
    };

    char   keyword[MAX__TOKEN__SIZE] = { 0 };
    size_t keyword__length           = 0;

    char current_character = 0;

    _lexer__rewind( file__tokenized );
    current_character = _lexer__peek( file__tokenized );

    keyword[keyword__length++] = current_character;

    _lexer__advance( file__tokenized );

    while ( ( current_character = _lexer__peek( file__tokenized ) ) != ':' &&
            current_character != '\n' && !_lexer__is_at_end( file__tokenized ) )
    {
        if ( keyword__length >= MAX__TOKEN__SIZE - 1 )
        {
            _LOGGING__PRINT__ERROR(
                "lexer: keyword scanned exceeds maximum token size. (maximum token size: %d)",
                MAX__TOKEN__SIZE );
            return token;
        }
        keyword[keyword__length++] = current_character;
        _lexer__advance( file__tokenized );
    }

    for ( uint8_t keyword_index = 0; _lexer__keywords[keyword_index] != NULL; keyword_index++ )
    {
        if ( strcmp( keyword, _lexer__keywords[keyword_index] ) == 0 )
        {
            token.type = (_lexer__token__type) keyword_index;
        }
    }

    return token;
}

_lexer__token _lexer__token__scan__arbitrary_value( _lexer__file__tokenized* file__tokenized )
{
    _lexer__token token = {
        .type   = _LEXER__TOKEN__TYPE__UNDEFINED,
        .value  = { 0 },
        .line   = file__tokenized->tokens__current_character__line,
        .column = file__tokenized->tokens__current_character__column,
    };

    char   arbitrary_value[MAX__TOKEN__SIZE] = { 0 };
    size_t arbitrary_value__length           = 0;

    char current_character = 0;

    while ( ( current_character = _lexer__peek( file__tokenized ) ) != '"' &&
            current_character != '\n' && !_lexer__is_at_end( file__tokenized ) )
    {
        if ( arbitrary_value__length >= MAX__TOKEN__SIZE - 1 )
        {
            _LOGGING__PRINT__ERROR(
                "lexer: keyword scanned exceeds maximum token size. (maximum token size: %d)",
                MAX__TOKEN__SIZE );
            return token;
        }
        arbitrary_value[arbitrary_value__length++] = current_character;
        _lexer__advance( file__tokenized );
    }

    if ( current_character == '"' )
    {
        token.type = _LEXER__TOKEN__TYPE__VALUE__ARBITRARY;
        strncpy( token.value, arbitrary_value, MAX__TOKEN__SIZE - 1 );
        token.value[arbitrary_value__length] = '\0';
    }

    return token;
}

_output__status _lexer__token__scan( _lexer__file__tokenized* file__tokenized )
{
    _lexer__token token                  = { 0 };
    _lexer__token token__arbitrary_value = { 0 };
    char          current_character      = _lexer__advance( file__tokenized );

    file__tokenized->tokens__current_character__column++;

    if ( isspace( current_character ) && current_character != '\n' )
        return _OUTPUT__STATUS__SUCCESS;

    switch ( current_character )
    {
        case EOF:
            token = _lexer__token__scan__character( file__tokenized,
                                                    _LEXER__TOKEN__TYPE__CHARACTER__EOF );
            break;
        case '\n':
            token = _lexer__token__scan__character( file__tokenized,
                                                    _LEXER__TOKEN__TYPE__CHARACTER__LINE_FEED );
            file__tokenized->tokens__current_character__line++;
            file__tokenized->tokens__current_character__column = 0;
            break;
        case ':':
            token = _lexer__token__scan__character( file__tokenized,
                                                    _LEXER__TOKEN__TYPE__CHARACTER__COLON );
            break;
        case '"':
            token = _lexer__token__scan__character(
                file__tokenized, _LEXER__TOKEN__TYPE__CHARACTER__QUOTATION_MARK );
            if ( file__tokenized->tokens[file__tokenized->tokens__total - 1].type !=
                 _LEXER__TOKEN__TYPE__VALUE__ARBITRARY )
            {
                token__arbitrary_value = _lexer__token__scan__arbitrary_value( file__tokenized );
            }
            break;
        case '[':
            token = _lexer__token__scan__character(
                file__tokenized, _LEXER__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__OPEN );
            break;
        case ']':
            token = _lexer__token__scan__character(
                file__tokenized, _LEXER__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE );
            break;
        case '@':
            token = _lexer__token__scan__character(
                file__tokenized, _LEXER__TOKEN__TYPE__CHARACTER__AT_THE_RATE_SIGN );
            break;
        case ',':
            token = _lexer__token__scan__character( file__tokenized,
                                                    _LEXER__TOKEN__TYPE__CHARACTER__COMMA );
            break;
        case '#':
            token = _lexer__token__scan__character( file__tokenized,
                                                    _LEXER__TOKEN__TYPE__CHARACTER__HASH );
            break;
        default:
            token = _lexer__token__scan__keyword( file__tokenized );
    };

    _output__status token__push_return_status                  = _OUTPUT__STATUS__FAILURE;
    _output__status token__arbitrary_value__push_return_status = _OUTPUT__STATUS__FAILURE;

    token__push_return_status = _lexer__token__push( file__tokenized, token );

    if ( token__arbitrary_value.type == _LEXER__TOKEN__TYPE__VALUE__ARBITRARY )
    {
        token__arbitrary_value__push_return_status =
            _lexer__token__push( file__tokenized, token__arbitrary_value );
    }
    else
    {
        token__arbitrary_value__push_return_status = _OUTPUT__STATUS__SUCCESS;
    }

    return ( token__push_return_status == _OUTPUT__STATUS__SUCCESS &&
             token__arbitrary_value__push_return_status == _OUTPUT__STATUS__SUCCESS )
               ? _OUTPUT__STATUS__SUCCESS
               : _OUTPUT__STATUS__FAILURE;
}

_output__status _lexer__file__tokenize( _lexer__file__tokenized* file__tokenized )
{
    while ( !_lexer__is_at_end( file__tokenized ) )
    {
        if ( _lexer__token__scan( file__tokenized ) == _OUTPUT__STATUS__FAILURE )
            return _OUTPUT__STATUS__FAILURE;
    }

    _lexer__token token__eof =
        _lexer__token__scan__character( file__tokenized, _LEXER__TOKEN__TYPE__CHARACTER__EOF );
    if ( _lexer__token__push( file__tokenized, token__eof ) == _OUTPUT__STATUS__FAILURE )
        return _OUTPUT__STATUS__FAILURE;

    return _OUTPUT__STATUS__SUCCESS;
}
