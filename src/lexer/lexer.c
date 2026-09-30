/*
 * lexer.c - functions and data structures for file tokenization
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

#include "input/file.h"
#include "lexer/lexer.h"
#include "logging/logging.h"

const char* const _bbfpm__token__type__keywords[] = { "NONE",         "NAME",         "REPOSITORY",
                                                      "VERSION",      "RELEASE-DATE", "AUTHORS",
                                                      "DEPENDENCIES", "LICENSE",      NULL };

_bbfpm__return_status _bbfpm__file__tokenized__initialize( _bbfpm__file__tokenized* file__tokenized,
                                                           const char*              file__path )
{
    memset( file__tokenized, 0, sizeof( _bbfpm__file__tokenized ) );

    _bbfpm__file* bbfpm_file = _bbfpm__open_bbfpm_file( file__path );
    if ( bbfpm_file == NULL )
        return _BBFPM__RETURN_STATUS__FAILURE;
    file__tokenized->file = bbfpm_file;

    static const uint64_t TOKENS__CAPACITY__INITIAL_VALUE = 10;
    file__tokenized->tokens__capacity                     = TOKENS__CAPACITY__INITIAL_VALUE;
    file__tokenized->tokens__total                        = 0;
    if ( ( file__tokenized->tokens =
               calloc( TOKENS__CAPACITY__INITIAL_VALUE, sizeof( _bbfpm__token ) ) ) == NULL )
    {
        _BBFPM__LOG__PRINT__FATAL( "lexer: failed allocating memory for _bbfpm__file__tokenized "
                                   "struct member: 'tokens'." );
        return _BBFPM__RETURN_STATUS__FAILURE;
    }

    return _BBFPM__RETURN_STATUS__SUCCESS;
}

void _bbfpm__file__tokenized__free( _bbfpm__file__tokenized* file__tokenized )
{
    _bbfpm__close_bbfpm_file( file__tokenized->file );
    free( file__tokenized->tokens );
}

bool _bbfpm__lexer__is_at_end( _bbfpm__file__tokenized* file__tokenized )
{
    if ( file__tokenized->tokens__current_character__position >= file__tokenized->file->size )
    {
        return true;
    }
    return false;
}

char _bbfpm__lexer__peek( _bbfpm__file__tokenized* file__tokenized )
{
    return file__tokenized->file->content[file__tokenized->tokens__current_character__position];
}

char _bbfpm__lexer__advance( _bbfpm__file__tokenized* file__tokenized )
{
    if ( !_bbfpm__lexer__is_at_end( file__tokenized ) )
    {
        return file__tokenized->file
            ->content[file__tokenized->tokens__current_character__position++];
    }
    return -1;
}

char _bbfpm__lexer__rewind( _bbfpm__file__tokenized* file__tokenized )
{
    if ( file__tokenized->tokens__current_character__position != 0 )
    {
        return file__tokenized->file
            ->content[--file__tokenized->tokens__current_character__position];
    }
    return -1;
}

char _bbfpm__lexer__peek_next( _bbfpm__file__tokenized* file__tokenized, uint64_t offset )
{
    if ( file__tokenized->file->size >
         file__tokenized->tokens__current_character__position + offset )
    {
        return file__tokenized->file
            ->content[file__tokenized->tokens__current_character__position + offset];
    }
    return -1;
}

bool _bbfpm__lexer__match( _bbfpm__file__tokenized* file__tokenized, char expected )
{
    if ( file__tokenized->file->content[file__tokenized->tokens__current_character__position] ==
         expected )
    {
        ( !_bbfpm__lexer__is_at_end( file__tokenized ) )
            ? ++file__tokenized->tokens__current_character__position
            : file__tokenized->tokens__current_character__position;
        return true;
    }
    return false;
}

_bbfpm__return_status _bbfpm__lexer__push_token( _bbfpm__file__tokenized* file__tokenized,
                                                 _bbfpm__token            token )
{
    if ( file__tokenized->tokens__capacity / 2 <= file__tokenized->tokens__total )
    {
        if ( ( file__tokenized->tokens =
                   reallocarray( file__tokenized->tokens, file__tokenized->tokens__capacity * 2,
                                 sizeof( _bbfpm__token ) ) ) == NULL )
        {
            _BBFPM__LOG__PRINT__FATAL( "lexer: couldn't reallocate allocated memory on "
                                       "_bbfpm__file__tokenized struct member: 'tokens'." );
            return _BBFPM__RETURN_STATUS__FAILURE;
        }
        file__tokenized->tokens__capacity *= 2;
    }
    file__tokenized->tokens[file__tokenized->tokens__total++] = token;
    return _BBFPM__RETURN_STATUS__SUCCESS;
}

_bbfpm__token _bbfpm__lexer__scan_token__character( _bbfpm__file__tokenized* file__tokenized,
                                                    _bbfpm__token__type      type )
{
    _bbfpm__token token = {
        .type   = type,
        .value  = { 0 },
        .line   = file__tokenized->tokens__current_character__line,
        .column = file__tokenized->tokens__current_character__column,
    };

    if ( type == _BBFPM__TOKEN__TYPE__CHARACTER__HASH )
    {
        while ( _bbfpm__lexer__peek( file__tokenized ) != '\n' &&
                !_bbfpm__lexer__is_at_end( file__tokenized ) )
        {
            _bbfpm__lexer__advance( file__tokenized );
        }
    }

    return token;
}

_bbfpm__token _bbfpm__lexer__scan_token__keyword( _bbfpm__file__tokenized* file__tokenized )
{
    _bbfpm__token token = {
        .type   = _BBFPM__TOKEN__TYPE__UNDEFINED,
        .value  = { 0 },
        .line   = file__tokenized->tokens__current_character__line,
        .column = file__tokenized->tokens__current_character__column,
    };

    char   keyword[MAX__TOKEN__SIZE] = { 0 };
    size_t keyword__length           = 0;

    char current_character = 0;

    _bbfpm__lexer__rewind( file__tokenized );
    current_character = _bbfpm__lexer__peek( file__tokenized );

    keyword[keyword__length++] = current_character;

    _bbfpm__lexer__advance( file__tokenized );

    while ( ( current_character = _bbfpm__lexer__peek( file__tokenized ) ) != ':' &&
            current_character != '\n' && !_bbfpm__lexer__is_at_end( file__tokenized ) )
    {
        if ( keyword__length >= MAX__TOKEN__SIZE - 1 )
        {
            _BBFPM__LOG__PRINT__ERROR(
                "lexer: keyword scanned exceeds maximum token size. (maximum token size: %d)",
                MAX__TOKEN__SIZE );
            return token;
        }
        keyword[keyword__length++] = current_character;
        _bbfpm__lexer__advance( file__tokenized );
    }

    for ( uint8_t keyword_index = 0; _bbfpm__token__type__keywords[keyword_index] != NULL;
          keyword_index++ )
    {
        if ( strcmp( keyword, _bbfpm__token__type__keywords[keyword_index] ) == 0 )
        {
            token.type = (_bbfpm__token__type) keyword_index;
        }
    }

    return token;
}

_bbfpm__token _bbfpm__lexer__scan_token__arbitrary_value( _bbfpm__file__tokenized* file__tokenized )
{
    _bbfpm__token token = {
        .type   = _BBFPM__TOKEN__TYPE__UNDEFINED,
        .value  = { 0 },
        .line   = file__tokenized->tokens__current_character__line,
        .column = file__tokenized->tokens__current_character__column,
    };

    char   arbitrary_value[MAX__TOKEN__SIZE] = { 0 };
    size_t arbitrary_value__length           = 0;

    char current_character = 0;

    while ( ( current_character = _bbfpm__lexer__peek( file__tokenized ) ) != '"' &&
            current_character != '\n' && !_bbfpm__lexer__is_at_end( file__tokenized ) )
    {
        if ( arbitrary_value__length >= MAX__TOKEN__SIZE - 1 )
        {
            _BBFPM__LOG__PRINT__ERROR(
                "lexer: keyword scanned exceeds maximum token size. (maximum token size: %d)",
                MAX__TOKEN__SIZE );
            return token;
        }
        arbitrary_value[arbitrary_value__length++] = current_character;
        _bbfpm__lexer__advance( file__tokenized );
    }

    if ( current_character == '"' )
    {
        token.type = _BBFPM__TOKEN__TYPE__VALUE__ARBITRARY;
        strncpy( token.value, arbitrary_value, MAX__TOKEN__SIZE - 1 );
        token.value[arbitrary_value__length] = '\0';
    }

    return token;
}

_bbfpm__return_status _bbfpm__lexer__scan_token( _bbfpm__file__tokenized* file__tokenized )
{
    _bbfpm__token token                  = { 0 };
    _bbfpm__token token__arbitrary_value = { 0 };
    char          current_character      = _bbfpm__lexer__advance( file__tokenized );

    file__tokenized->tokens__current_character__column++;

    if ( isspace( current_character ) && current_character != '\n' )
        return _BBFPM__RETURN_STATUS__SUCCESS;

    switch ( current_character )
    {
        case EOF:
            token = _bbfpm__lexer__scan_token__character( file__tokenized,
                                                          _BBFPM__TOKEN__TYPE__CHARACTER__EOF );
            break;
        case '\n':
            token = _bbfpm__lexer__scan_token__character(
                file__tokenized, _BBFPM__TOKEN__TYPE__CHARACTER__LINE_FEED );
            file__tokenized->tokens__current_character__line++;
            file__tokenized->tokens__current_character__column = 0;
            break;
        case ':':
            token = _bbfpm__lexer__scan_token__character( file__tokenized,
                                                          _BBFPM__TOKEN__TYPE__CHARACTER__COLON );
            break;
        case '"':
            token = _bbfpm__lexer__scan_token__character(
                file__tokenized, _BBFPM__TOKEN__TYPE__CHARACTER__QUOTATION_MARK );
            if ( file__tokenized->tokens[file__tokenized->tokens__total - 1].type !=
                 _BBFPM__TOKEN__TYPE__VALUE__ARBITRARY )
            {
                token__arbitrary_value =
                    _bbfpm__lexer__scan_token__arbitrary_value( file__tokenized );
            }
            break;
        case '[':
            token = _bbfpm__lexer__scan_token__character(
                file__tokenized, _BBFPM__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__OPEN );
            break;
        case ']':
            token = _bbfpm__lexer__scan_token__character(
                file__tokenized, _BBFPM__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE );
            break;
        case '@':
            token = _bbfpm__lexer__scan_token__character(
                file__tokenized, _BBFPM__TOKEN__TYPE__CHARACTER__AT_THE_RATE_SIGN );
            break;
        case ',':
            token = _bbfpm__lexer__scan_token__character( file__tokenized,
                                                          _BBFPM__TOKEN__TYPE__CHARACTER__COMMA );
            break;
        case '#':
            token = _bbfpm__lexer__scan_token__character( file__tokenized,
                                                          _BBFPM__TOKEN__TYPE__CHARACTER__HASH );
            break;
        default:
            token = _bbfpm__lexer__scan_token__keyword( file__tokenized );
    };

    _bbfpm__return_status token__push_return_status = _BBFPM__RETURN_STATUS__FAILURE;
    _bbfpm__return_status token__arbitrary_value__push_return_status =
        _BBFPM__RETURN_STATUS__FAILURE;

    token__push_return_status = _bbfpm__lexer__push_token( file__tokenized, token );

    if ( token__arbitrary_value.type == _BBFPM__TOKEN__TYPE__VALUE__ARBITRARY )
    {
        token__arbitrary_value__push_return_status =
            _bbfpm__lexer__push_token( file__tokenized, token__arbitrary_value );
    }
    else
    {
        token__arbitrary_value__push_return_status = _BBFPM__RETURN_STATUS__SUCCESS;
    }

    return ( token__push_return_status == _BBFPM__RETURN_STATUS__SUCCESS &&
             token__arbitrary_value__push_return_status == _BBFPM__RETURN_STATUS__SUCCESS )
               ? _BBFPM__RETURN_STATUS__SUCCESS
               : _BBFPM__RETURN_STATUS__FAILURE;
}

_bbfpm__return_status _bbfpm__file__tokenize( _bbfpm__file__tokenized* file__tokenized )
{
    while ( !_bbfpm__lexer__is_at_end( file__tokenized ) )
    {
        if ( _bbfpm__lexer__scan_token( file__tokenized ) == _BBFPM__RETURN_STATUS__FAILURE )
            return _BBFPM__RETURN_STATUS__FAILURE;
    }

    _bbfpm__token token__eof = _bbfpm__lexer__scan_token__character(
        file__tokenized, _BBFPM__TOKEN__TYPE__CHARACTER__EOF );
    if ( _bbfpm__lexer__push_token( file__tokenized, token__eof ) ==
         _BBFPM__RETURN_STATUS__FAILURE )
        return _BBFPM__RETURN_STATUS__FAILURE;

    return _BBFPM__RETURN_STATUS__SUCCESS;
}
