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

bool _lexer__is_at_end( _lexer__file__tokenized* file__tokenized )
{
    if ( file__tokenized->tokens__current_character__position >= file__tokenized->file->size )
    {
        return true;
    }
    return false;
}

char _lexer__peek( _lexer__file__tokenized* file__tokenized )
{
    return file__tokenized->file->content[file__tokenized->tokens__current_character__position];
}

char _lexer__advance( _lexer__file__tokenized* file__tokenized )
{
    if ( !_lexer__is_at_end( file__tokenized ) )
    {
        return file__tokenized->file
            ->content[file__tokenized->tokens__current_character__position++];
    }
    return -1;
}

char _lexer__rewind( _lexer__file__tokenized* file__tokenized )
{
    if ( file__tokenized->tokens__current_character__position != 0 )
    {
        return file__tokenized->file
            ->content[--file__tokenized->tokens__current_character__position];
    }
    return -1;
}

char _lexer__peek_next( _lexer__file__tokenized* file__tokenized, uint64_t offset )
{
    if ( file__tokenized->file->size >
         file__tokenized->tokens__current_character__position + offset )
    {
        return file__tokenized->file
            ->content[file__tokenized->tokens__current_character__position + offset];
    }
    return -1;
}

bool _lexer__match( _lexer__file__tokenized* file__tokenized, char expected )
{
    if ( file__tokenized->file->content[file__tokenized->tokens__current_character__position] ==
         expected )
    {
        ( !_lexer__is_at_end( file__tokenized ) )
            ? ++file__tokenized->tokens__current_character__position
            : file__tokenized->tokens__current_character__position;
        return true;
    }
    return false;
}
