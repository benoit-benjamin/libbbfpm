/*
 * primitives.c - primitive functions for moving through tokenized files
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

#include "parser/primitives.h"
#include "lexer/types.h"
#include "parser/types.h"

bool _parser__is_at_end( _parser__file__parsed* file__parsed )
{
    if ( file__parsed->file__tokenized->tokens__current_token__position >=
         file__parsed->file__tokenized->tokens__total )
    {
        return true;
    }
    return false;
}

_lexer__token _parser__peek( _parser__file__parsed* file__parsed )
{
    return file__parsed->file__tokenized
        ->tokens[file__parsed->file__tokenized->tokens__current_token__position];
}

_lexer__token _parser__advance( _parser__file__parsed* file__parsed )
{
    if ( !_parser__is_at_end( file__parsed ) )
    {
        return file__parsed->file__tokenized
            ->tokens[file__parsed->file__tokenized->tokens__current_token__position++];
    }
    return (_lexer__token) {
        .type = _LEXER__TOKEN__TYPE__UNDEFINED, .column = 0, .line = 0, .value = { 0 }
    };
}

_lexer__token _parser__peek_next( _parser__file__parsed* file__parsed, uint64_t offset )
{
    if ( file__parsed->file__tokenized->tokens__total >
         file__parsed->file__tokenized->tokens__current_token__position + offset )
    {
        return file__parsed->file__tokenized
            ->tokens[file__parsed->file__tokenized->tokens__current_token__position + offset];
    }
    return (_lexer__token) {
        .type = _LEXER__TOKEN__TYPE__UNDEFINED, .column = 0, .line = 0, .value = { 0 }
    };
}

bool _parser__match( _parser__file__parsed* file__parsed, _lexer__token expected )
{
    if ( file__parsed->file__tokenized
             ->tokens[file__parsed->file__tokenized->tokens__current_token__position]
             .type == expected.type )
    {
        ( !_parser__is_at_end( file__parsed ) )
            ? ++file__parsed->file__tokenized->tokens__current_token__position
            : file__parsed->file__tokenized->tokens__current_token__position;
        return true;
    }
    return false;
}
