/*
 * lexer.h - functions for file tokenization
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

#ifndef BBFPM__LEXER__LEXER__H
#define BBFPM__LEXER__LEXER__H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lexer/types.h"
#include "output/types.h"

_output__status _lexer__token__push( _lexer__file__tokenized* file__tokenized,
                                     _lexer__token            token );
_lexer__token   _lexer__token__scan__character( _lexer__file__tokenized* file__tokenized,
                                                _lexer__token__type      type );
_lexer__token   _lexer__token__scan__keyword( _lexer__file__tokenized* file__tokenizied );
_lexer__token   _lexer__token__scan__arbitrary_value( _lexer__file__tokenized* file__tokenized );
_output__status _lexer__token__scan( _lexer__file__tokenized* file__tokenized );

_output__status _lexer__file__tokenize( _lexer__file__tokenized* file__tokenized );

#endif
