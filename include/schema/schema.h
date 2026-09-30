/*
 * schema.h - functions for schema validation
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

#ifndef BBFPM__SCHEMA__SCHEMA__H
#define BBFPM__SCHEMA__SCHEMA__H

#include <stdbool.h>
#include <stddef.h>

#include "parser/types.h"

bool _schema__validate( _parser__file__parsed* file__parsed );

#endif
