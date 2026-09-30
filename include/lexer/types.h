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

typedef enum _bbfpm__token__type
{
    _BBFPM__TOKEN__TYPE__UNDEFINED                        = -1,
    _BBFPM__TOKEN__TYPE__KEYWORD__NONE                    = 0,
    _BBFPM__TOKEN__TYPE__KEYWORD__NAME                    = 1,
    _BBFPM__TOKEN__TYPE__KEYWORD__REPOSITORY              = 2,
    _BBFPM__TOKEN__TYPE__KEYWORD__VERSION                 = 3,
    _BBFPM__TOKEN__TYPE__KEYWORD__RELEASE_DATE            = 4,
    _BBFPM__TOKEN__TYPE__KEYWORD__AUTHORS                 = 5,
    _BBFPM__TOKEN__TYPE__KEYWORD__DEPENDENCIES            = 6,
    _BBFPM__TOKEN__TYPE__KEYWORD__LICENSE                 = 7,
    _BBFPM__TOKEN__TYPE__KEYWORDS_LIMIT                   = 19,
    _BBFPM__TOKEN__TYPE__CHARACTER__EOF                   = 20,
    _BBFPM__TOKEN__TYPE__CHARACTER__LINE_FEED             = 21,
    _BBFPM__TOKEN__TYPE__CHARACTER__COLON                 = 22,
    _BBFPM__TOKEN__TYPE__CHARACTER__QUOTATION_MARK        = 23,
    _BBFPM__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__OPEN  = 24,
    _BBFPM__TOKEN__TYPE__CHARACTER__SQUARE_BRACKET__CLOSE = 25,
    _BBFPM__TOKEN__TYPE__CHARACTER__AT_THE_RATE_SIGN      = 26,
    _BBFPM__TOKEN__TYPE__CHARACTER__COMMA                 = 27,
    _BBFPM__TOKEN__TYPE__CHARACTER__HASH                  = 28,
    _BBFPM__TOKEN__TYPE__CHARACTERS_LIMIT                 = 39,
    _BBFPM__TOKEN__TYPE__VALUE__ARBITRARY                 = 40,
} _bbfpm__token__type;

extern const char* const _bbfpm__token__type__keywords[];

typedef struct _bbfpm__token
{
    _bbfpm__token__type type;
    char                value[MAX__TOKEN__SIZE];
    uint32_t            line;
    uint32_t            column;
} _bbfpm__token;

typedef struct _bbfpm__file__tokenized
{
    _bbfpm__file*  file;
    _bbfpm__token* tokens;
    uint64_t       tokens__capacity;
    uint64_t       tokens__total;
    uint64_t       tokens__current_token__position;
    uint64_t       tokens__current_character__position;
    uint32_t       tokens__current_character__line;
    uint32_t       tokens__current_character__column;
} _bbfpm__file__tokenized;

_bbfpm__return_status _bbfpm__file__tokenized__initialize( _bbfpm__file__tokenized* file__tokenized,
                                                           const char*              file__path );
void                  _bbfpm__file__tokenized__free( _bbfpm__file__tokenized* file__tokenized );

#endif
