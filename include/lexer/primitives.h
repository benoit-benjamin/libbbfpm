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

bool _bbfpm__lexer__is_at_end( _bbfpm__file__tokenized* file__tokenized );
char _bbfpm__lexer__peek( _bbfpm__file__tokenized* file__tokenized );
char _bbfpm__lexer__advance( _bbfpm__file__tokenized* file__tokenized );
char _bbfpm__lexer__rewind( _bbfpm__file__tokenized* file__tokenized );
char _bbfpm__lexer__peek_next( _bbfpm__file__tokenized* file__tokenized, uint64_t offset );
bool _bbfpm__lexer__match( _bbfpm__file__tokenized* file__tokenized, char expected );

#endif
