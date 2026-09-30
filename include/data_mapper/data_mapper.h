/*
 * data_mapper.h - functions for parsed and validated data
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

#ifndef BBFPM__DATA_MAPPER__H
#define BBFPM__DATA_MAPPER__H

#include "data_mapper/types.h"
#include "output/types.h"
#include "parser/types.h"

_output__status _data_mapper__map_metadata( _parser__file__parsed*  file__parsed,
                                            _data_mapper__metadata* mapped_metadata );

_output__status _data_mapper__metadata__free( _data_mapper__metadata* mapped_metadata,
                                              _parser__file__parsed*  file__parsed );

#endif
