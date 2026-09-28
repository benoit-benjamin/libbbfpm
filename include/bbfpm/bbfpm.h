/*
 * bbfpm.h - public API of the libcbbfpm library
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

#ifndef BBFPM__BBFPM__H
#define BBFPM__BBFPM__H

#include "export.h"
#include <stdint.h>

typedef struct BBFPM__CorrelatedArbitraryValue
{
    char* value;
    char* correlation;
} BBFPM__CorrelatedArbitraryValue;

typedef struct BBFPM__CorrelatedArbitraryValueList
{
    BBFPM__CorrelatedArbitraryValue* correlations;
    uint32_t                         correlations__total;
} BBFPM__CorrelatedArbitraryValueList;

typedef struct BBFPM__Metadata BBFPM__Metadata;

BBFPM__API BBFPM__Metadata* BBFPM__LoadMetadata( const char* file_path );
BBFPM__API void             BBFPM__DestroyMetadata( BBFPM__Metadata* metadata );

BBFPM__API char* BBFPM__GetMetadataName( BBFPM__Metadata* metadata );
BBFPM__API char* BBFPM__GetMetadataRepository( BBFPM__Metadata* metadata );
BBFPM__API char* BBFPM__GetMetadataVersion( BBFPM__Metadata* metadata );
BBFPM__API char* BBFPM__GetMetadataReleaseDate( BBFPM__Metadata* metadata );
BBFPM__API char* BBFPM__GetMetadataLicense( BBFPM__Metadata* metadata );
BBFPM__API BBFPM__CorrelatedArbitraryValueList
BBFPM__GetMetadataAuthors( BBFPM__Metadata* metadata );
BBFPM__API BBFPM__CorrelatedArbitraryValueList
BBFPM__GetMetadataDependencies( BBFPM__Metadata* metadata );

BBFPM__API void
BBFPM__DestroyCorrelatedArbitraryValueList( BBFPM__CorrelatedArbitraryValueList* list );

#endif
