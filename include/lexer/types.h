/*
 * types.h - data structures and functions for tokenized files
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

#ifndef BBFPM__LEXER__TYPES__H
#define BBFPM__LEXER__TYPES__H

#include <stdint.h>

#include "input/file.h"
#include "output/types.h"

#define MAX__TOKEN__SIZE 64

typedef enum _lexer__token__type
{
    _LEXER__TOKEN__TYPE__UNDEFINED                        = -1,
    _LEXER__TOKEN__TYPE__KEYWORD__NONE                    = 0,
    _LEXER__TOKEN__TYPE__KEYWORD__NAME                    = 1,
    _LEXER__TOKEN__TYPE__KEYWORD__REPOSITORY              = 2,
    _LEXER__TOKEN__TYPE__KEYWORD__VERSION                 = 3,
    _LEXER__TOKEN__TYPE__KEYWORD__RELEASE_DATE            = 4,
    _LEXER__TOKEN__TYPE__KEYWORD__AUTHORS                 = 5,
    _LEXER__TOKEN__TYPE__KEYWORD__DEPENDENCIES            = 6,
    _LEXER__TOKEN__TYPE__KEYWORD__LICENSE                 = 7,
    _LEXER__TOKEN__TYPE__KEYWORDS_LIMIT                   = 19,
    _LEXER__TOKEN__TYPE__CHARACTER__EOF                   = 20,
    _LEXER__TOKEN__TYPE__CHARACTER__LINE_FEED             = 21,
    _LEXER__TOKEN__TYPE__CHARACTER__COLON                 = 22,
    _LEXER__TOKEN__TYPE__CHARACTER__QUOTATION_MARK        = 23,
    _LEXER__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__OPEN  = 24,
    _LEXER__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE = 25,
    _LEXER__TOKEN__TYPE__CHARACTER__AT_THE_RATE_SIGN      = 26,
    _LEXER__TOKEN__TYPE__CHARACTER__COMMA                 = 27,
    _LEXER__TOKEN__TYPE__CHARACTER__HASH                  = 28,
    _LEXER__TOKEN__TYPE__CHARACTERS_LIMIT                 = 39,
    _LEXER__TOKEN__TYPE__VALUE__ARBITRARY                 = 40,
} _lexer__token__type;

extern const char* const _lexer__keywords[];

typedef struct _lexer__token
{
    _lexer__token__type type;
    char                value[MAX__TOKEN__SIZE];
    uint32_t            line;
    uint32_t            column;
} _lexer__token;

typedef struct _lexer__file__tokenized
{
    _input__file*  file;
    _lexer__token* tokens;
    uint64_t       tokens__capacity;
    uint64_t       tokens__total;
    uint64_t       tokens__current_token__position;
    uint64_t       tokens__current_character__position;
    uint32_t       tokens__current_character__line;
    uint32_t       tokens__current_character__column;
} _lexer__file__tokenized;

_output__status _lexer__file__tokenized__initialize( _lexer__file__tokenized* file__tokenized,
                                                     const char*              file__path );
void            _lexer__file__tokenized__free( _lexer__file__tokenized* file__tokenized );

#endif
