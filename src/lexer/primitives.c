/*
 * primitives.c - primitive functions for moving through the raw content of a file
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

#include "lexer/primitives.h"
#include "lexer/types.h"

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
