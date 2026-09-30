/*
 * parser.h - functions for parsing tokenized files
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

#ifndef BBFPM__PARSER__PARSER__H
#define BBFPM__PARSER__PARSER__H

#include "output/types.h"
#include "parser/types.h"

_output__status _parser__key__push( _parser__file__parsed* file__parsed, _parser__key key );
_output__status _parser__key__parse__arbitrary( _parser__file__parsed* file__parsed,
                                                _parser__key*          key );
_output__status _parser__key__parse__list_correlated_arbitrary( _parser__file__parsed* file__parsed,
                                                                _parser__key*          key );
_output__status _parser__key__parse__none( _parser__file__parsed* file__parsed, _parser__key* key );
_parser__key    _parser__key__parse( _parser__file__parsed* file__parsed );

_output__status _parser__file__parsed__parse( _parser__file__parsed* file__parsed,
                                              const char*            file__path );


#endif
