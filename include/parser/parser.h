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

_bbfpm__return_status _bbfpm__parser__push_key( _bbfpm__file__parsed* file__parsed,
                                                _bbfpm__key           key );
_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__arbitrary( _bbfpm__file__parsed* file__parsed,
                                                 _bbfpm__key*          key );
_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__list_correlated_arbitrary( _bbfpm__file__parsed* file__parsed,
                                                                 _bbfpm__key*          key );
_bbfpm__return_status
_bbfpm__parser__parse_key__key__type__none( _bbfpm__file__parsed* file__parsed, _bbfpm__key* key );
_bbfpm__key _bbfpm__parser__parse_key( _bbfpm__file__parsed* file__parsed );

_bbfpm__return_status _bbfpm__file__parsed__parse( _bbfpm__file__parsed* file__parsed,
                                                   const char*           file__path );


#endif
