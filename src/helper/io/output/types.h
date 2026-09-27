/*
 * types.h - internal data types for representing output states
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

#ifndef BBFPM__TYPES__H
#define BBFPM__TYPES__H

typedef enum _bbfpm__return_status
{
    _BBFPM__RETURN_STATUS__FAILURE,
    _BBFPM__RETURN_STATUS__SUCCESS,
} _bbfpm__return_status;

#endif
