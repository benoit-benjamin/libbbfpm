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

const char* const _lexer__keywords[] = { "NONE",         "NAME",         "REPOSITORY",
                                         "VERSION",      "RELEASE-DATE", "AUTHORS",
                                         "DEPENDENCIES", "LICENSE",      NULL };

_output__status _lexer__file__tokenized__initialize( _lexer__file__tokenized* file__tokenized,
                                                     const char*              file__path )
{
    memset( file__tokenized, 0, sizeof( _lexer__file__tokenized ) );

    _input__file* bbfpm_file = _input__file__open( file__path );
    if ( bbfpm_file == NULL )
        return _OUTPUT__STATUS__FAILURE;
    file__tokenized->file = bbfpm_file;

    static const uint64_t TOKENS__CAPACITY__INITIAL_VALUE = 10;
    file__tokenized->tokens__capacity                     = TOKENS__CAPACITY__INITIAL_VALUE;
    file__tokenized->tokens__total                        = 0;
    if ( ( file__tokenized->tokens =
               calloc( TOKENS__CAPACITY__INITIAL_VALUE, sizeof( _lexer__token ) ) ) == NULL )
    {
        _LOGGING__PRINT__FATAL( "lexer: failed allocating memory for _lexer__file__tokenized "
                                "struct member: 'tokens'." );
        return _OUTPUT__STATUS__FAILURE;
    }

    return _OUTPUT__STATUS__SUCCESS;
}

void _lexer__file__tokenized__free( _lexer__file__tokenized* file__tokenized )
{
    _input__file__close( file__tokenized->file );
    free( file__tokenized->tokens );
}
