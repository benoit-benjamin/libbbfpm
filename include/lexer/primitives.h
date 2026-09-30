/*
 * primitives.h - primitive functions for moving through the raw content of a file
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

#ifndef BBFPM__LEXER__PRIMITIVES__H
#define BBFPM__LEXER__PRIMITIVES__H

#include <stdbool.h>

#include "lexer/types.h"

bool _lexer__is_at_end( _lexer__file__tokenized* file__tokenized );
char _lexer__peek( _lexer__file__tokenized* file__tokenized );
char _lexer__advance( _lexer__file__tokenized* file__tokenized );
char _lexer__rewind( _lexer__file__tokenized* file__tokenized );
char _lexer__peek_next( _lexer__file__tokenized* file__tokenized, uint64_t offset );
bool _lexer__match( _lexer__file__tokenized* file__tokenized, char expected );

#endif
