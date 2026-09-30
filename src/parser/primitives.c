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
