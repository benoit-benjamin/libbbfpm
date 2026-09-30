/*
 * types.c - data structures and functions for tokenized files
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

#include <stdlib.h>
#include <string.h>

#include "lexer/types.h"
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
