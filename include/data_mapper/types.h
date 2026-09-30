#ifndef BBFPM__DATA_MAPPER__TYPES__H
#define BBFPM__DATA_MAPPER__TYPES__H

#include <stdint.h>

typedef struct _data_mapper__entry
{
    void*    value;
    uint32_t extent;
} _data_mapper__entry;

typedef struct _data_mapper__metadata
{
    _data_mapper__entry name;
    _data_mapper__entry repository;
    _data_mapper__entry version;
    _data_mapper__entry release_date;
    _data_mapper__entry license;
    _data_mapper__entry authors;
    _data_mapper__entry dependencies;
} _data_mapper__metadata;

#endif
