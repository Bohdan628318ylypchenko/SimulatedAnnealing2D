#pragma once

#include "V2.h"

typedef struct
{
    size_t capacity;
    size_t count;
    V2 path[];
} OptPath;

OptPath * OptPath_New(size_t capacity);

OptPath * OptPath_Append(OptPath * optPath, V2 v2);

OptPath * OptPath_Free(OptPath * optPath);
