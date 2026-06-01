#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct {
    char* path;
    uint32_t size;
} File;

typedef struct {
    File* file;
    size_t count;
    size_t capacity;
} FileList;
