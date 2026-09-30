#ifndef BBFPM__DATA_MAPPER__TYPES__H
#define BBFPM__DATA_MAPPER__TYPES__H

#include <stdint.h>

typedef struct _bbfpm__data__entry
{
    void*    value;
    uint32_t extent;
} _bbfpm__data__entry;

typedef struct _bbfpm__data
{
    _bbfpm__data__entry name;
    _bbfpm__data__entry repository;
    _bbfpm__data__entry version;
    _bbfpm__data__entry release_date;
    _bbfpm__data__entry license;
    _bbfpm__data__entry authors;
    _bbfpm__data__entry dependencies;
} _bbfpm__mapped_metadata;

#endif
