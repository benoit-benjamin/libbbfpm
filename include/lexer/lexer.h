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

_bbfpm__return_status _bbfpm__lexer__push_token( _bbfpm__file__tokenized* file__tokenized,
                                                 _bbfpm__token            token );
_bbfpm__token _bbfpm__lexer__scan_token__character( _bbfpm__file__tokenized* file__tokenized,
                                                    _bbfpm__token__type      type );
_bbfpm__token _bbfpm__lexer__scan_token__keyword( _bbfpm__file__tokenized* file__tokenizied );
_bbfpm__token
_bbfpm__lexer__scan_token__arbitrary_value( _bbfpm__file__tokenized* file__tokenized );
_bbfpm__return_status _bbfpm__lexer__scan_token( _bbfpm__file__tokenized* file__tokenized );

_bbfpm__return_status _bbfpm__file__tokenize( _bbfpm__file__tokenized* file__tokenized );

#endif
