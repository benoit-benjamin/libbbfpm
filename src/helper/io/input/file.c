/*
 * file.c - BBFPM file operations and raw data representation
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "logging.h"

_bbfpm__file* _bbfpm__open_bbfpm_file( const char* file_path )
{
    _bbfpm__file* raw_bbfpm_file = malloc( sizeof( _bbfpm__file ) );
    FILE*         file_pointer   = NULL;
    if ( ( file_pointer = fopen( file_path, "rb" ) ) == NULL )
    {
        _BBFPM__LOG__PRINT__ERROR( "failed to open the provided file: %s. errno: %s", file_path,
                                   strerror( errno ) );
        goto on_error;
    }

    if ( fseek( file_pointer, 0, SEEK_END ) == -1 )
    {
        _BBFPM__LOG__PRINT__ERROR( "failed seeking opened file: %s (%p) to it's end. errno: %s",
                                   file_path, file_pointer, strerror( errno ) );
        goto on_error;
    }

    if ( ( raw_bbfpm_file->size = ftell( file_pointer ) ) == -1 )
    {
        _BBFPM__LOG__PRINT__ERROR( "failed getting the current value of the file position "
                                   "indicator in the opened file: %s (%p). errno: %s",
                                   file_path, file_pointer, strerror( errno ) );
        goto on_error;
    }

    rewind( file_pointer );

    if ( ( raw_bbfpm_file->content = malloc( sizeof( char ) * raw_bbfpm_file->size + 1 ) ) == NULL )
    {
        _BBFPM__LOG__PRINT__ERROR(
            "failed allocating memory for _bbfpm__file struct member: 'content'. errno: %s",
            strerror( errno ) );
        goto on_error;
    }

    memset( raw_bbfpm_file->content, 0, raw_bbfpm_file->size );

    if ( fread( raw_bbfpm_file->content, 1, raw_bbfpm_file->size, file_pointer ) !=
         raw_bbfpm_file->size )
    {
        _BBFPM__LOG__PRINT__ERROR( "failed reading opened file and storing it. Chunk size: 1. "
                                   "Total chunks: %ld. errno: %s",
                                   raw_bbfpm_file->size, strerror( errno ) );
        goto on_error;
    }

    return raw_bbfpm_file;

on_error:
    fclose( file_pointer );
    _bbfpm__close_bbfpm_file( raw_bbfpm_file );
    return NULL;
}

void _bbfpm__close_bbfpm_file( _bbfpm__file* file )
{
    free( file->content );
    free( file );
}
