/*
 * bbfpm.c - public API of the libcbbfpm library
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

#include <stdlib.h>
#include <string.h>

#include "bbfpm.h"
#include "data_mapper.h"
#include "parser.h"
#include "schema.h"

typedef struct BBFPM__Metadata
{
    _bbfpm__data__entry name;
    _bbfpm__data__entry repository;
    _bbfpm__data__entry version;
    _bbfpm__data__entry release_date;
    _bbfpm__data__entry license;
    _bbfpm__data__entry authors;
    _bbfpm__data__entry dependencies;
} BBFPM__Metadata;

BBFPM__Metadata* BBFPM__LoadMetadata( const char* file__path )
{
    BBFPM__Metadata* metadata = malloc( sizeof( BBFPM__Metadata ) );
    memset( metadata, 0, sizeof( BBFPM__Metadata ) );

    _bbfpm__file__parsed file__parsed = { 0 };
    if ( _bbfpm__file__parsed__parse( &file__parsed, file__path ) ==
         _BBFPM__RETURN_STATUS__FAILURE )
        return NULL;

    if ( _bbfpm__verify__file__parsed__through_schema( &file__parsed ) ==
         _BBFPM__RETURN_STATUS__FAILURE )
        return NULL;

    _bbfpm__mapped_metadata mapped_metadata = { 0 };
    if ( _bbfpm__map_metadata_from__file__parsed( &file__parsed, &mapped_metadata ) ==
         _BBFPM__RETURN_STATUS__FAILURE )
        return NULL;

    metadata->name.value          = ( mapped_metadata.name.value != NULL )
                                        ? strdup( (char*) mapped_metadata.name.value )
                                        : NULL;
    metadata->name.extent         = mapped_metadata.name.extent;
    metadata->repository.value    = ( mapped_metadata.repository.value != NULL )
                                        ? strdup( (char*) mapped_metadata.repository.value )
                                        : NULL;
    metadata->repository.extent   = mapped_metadata.repository.extent;
    metadata->version.value       = ( mapped_metadata.version.value != NULL )
                                        ? strdup( (char*) mapped_metadata.version.value )
                                        : NULL;
    metadata->version.extent      = mapped_metadata.version.extent;
    metadata->release_date.value  = ( mapped_metadata.release_date.value != NULL )
                                        ? strdup( (char*) mapped_metadata.release_date.value )
                                        : NULL;
    metadata->release_date.extent = mapped_metadata.release_date.extent;

    metadata->license.value  = ( mapped_metadata.license.value != NULL )
                                   ? strdup( (char*) mapped_metadata.license.value )
                                   : NULL;
    metadata->license.extent = mapped_metadata.license.extent;

    if ( mapped_metadata.authors.value != NULL )
    {
        metadata->authors.value = calloc( mapped_metadata.authors.extent, sizeof( char* ) );
        for ( uint32_t index__authors = 0; index__authors < mapped_metadata.authors.extent;
              index__authors++ )
            ( (char**) metadata->authors.value )[index__authors] =
                strdup( ( (char**) mapped_metadata.authors.value )[index__authors] );
        metadata->authors.extent = mapped_metadata.authors.extent;
    }
    else
    {
        metadata->authors.value  = NULL;
        metadata->authors.extent = 0;
    }

    if ( mapped_metadata.dependencies.value != NULL )
    {
        metadata->dependencies.value =
            calloc( mapped_metadata.dependencies.extent, sizeof( char* ) );
        for ( uint32_t index__dependencies = 0;
              index__dependencies < mapped_metadata.dependencies.extent; index__dependencies++ )
            ( (char**) metadata->dependencies.value )[index__dependencies] =
                strdup( ( (char**) mapped_metadata.dependencies.value )[index__dependencies] );
        metadata->dependencies.extent = mapped_metadata.dependencies.extent;
    }
    else
    {
        metadata->dependencies.value  = NULL;
        metadata->dependencies.extent = 0;
    }

    _bbfpm__mapped_metadata__free( &mapped_metadata, &file__parsed );

    return metadata;
}

void BBFPM__DestroyMetadata( BBFPM__Metadata* metadata )
{
    free( metadata->name.value );
    metadata->name.value  = NULL;
    metadata->name.extent = 0;

    free( metadata->repository.value );
    metadata->repository.value  = NULL;
    metadata->repository.extent = 0;

    free( metadata->version.value );
    metadata->version.value  = NULL;
    metadata->version.extent = 0;

    free( metadata->release_date.value );
    metadata->release_date.value  = NULL;
    metadata->release_date.extent = 0;

    for ( uint32_t index__authors = 0; index__authors < metadata->authors.extent; index__authors++ )
        free( ( (char**) metadata->authors.value )[index__authors] );
    free( (char**) metadata->authors.value );
    metadata->authors.value  = NULL;
    metadata->authors.extent = 0;

    for ( uint32_t index__dependencies = 0; index__dependencies < metadata->dependencies.extent;
          index__dependencies++ )
        free( ( (char**) metadata->dependencies.value )[index__dependencies] );
    free( (char**) metadata->dependencies.value );
    metadata->dependencies.value  = NULL;
    metadata->dependencies.extent = 0;

    free( metadata );
    metadata = NULL;
}

char* BBFPM__GetMetadataName( BBFPM__Metadata* metadata )
{
    return (char*) metadata->name.value;
}
char* BBFPM__GetMetadataRepository( BBFPM__Metadata* metadata )
{
    return (char*) metadata->repository.value;
}
char* BBFPM__GetMetadataVersion( BBFPM__Metadata* metadata )
{
    return (char*) metadata->version.value;
}
char* BBFPM__GetMetadataReleaseDate( BBFPM__Metadata* metadata )
{
    return (char*) metadata->release_date.value;
}
char* BBFPM__GetMetadataLicense( BBFPM__Metadata* metadata )
{
    return (char*) metadata->license.value;
}

BBFPM__CorrelatedArbitraryValueList BBFPM__GetMetadataAuthors( BBFPM__Metadata* metadata )
{
    BBFPM__CorrelatedArbitraryValueList list = { 0 };
    list.correlations =
        calloc( metadata->authors.extent / 2, sizeof( BBFPM__CorrelatedArbitraryValue ) );
    for ( uint32_t index__arbitrary_value = 0; index__arbitrary_value < metadata->authors.extent;
          index__arbitrary_value++ )
    {
        BBFPM__CorrelatedArbitraryValue current_correlated_value = { 0 };
        current_correlated_value.value =
            ( (char**) metadata->authors.value )[index__arbitrary_value++];
        current_correlated_value.correlation =
            ( (char**) metadata->authors.value )[index__arbitrary_value];
        list.correlations[list.correlations__total++] = current_correlated_value;
    }
    return list;
}
BBFPM__CorrelatedArbitraryValueList BBFPM__GetMetadataDependencies( BBFPM__Metadata* metadata )
{
    BBFPM__CorrelatedArbitraryValueList list = { 0 };
    if ( metadata->dependencies.value == NULL )
        return list;
    list.correlations =
        calloc( metadata->dependencies.extent / 2, sizeof( BBFPM__CorrelatedArbitraryValue ) );
    for ( uint32_t index__arbitrary_value = 0;
          index__arbitrary_value < metadata->dependencies.extent; index__arbitrary_value++ )
    {
        BBFPM__CorrelatedArbitraryValue current_correlated_value = { 0 };
        current_correlated_value.value =
            ( (char**) metadata->dependencies.value )[index__arbitrary_value++];
        current_correlated_value.correlation =
            ( (char**) metadata->dependencies.value )[index__arbitrary_value];
        list.correlations[list.correlations__total++] = current_correlated_value;
    }
    return list;
}

void BBFPM__DestroyCorrelatedArbitraryValueList( BBFPM__CorrelatedArbitraryValueList* list )
{
    free( list->correlations );
    list->correlations        = NULL;
    list->correlations__total = 0;
}
