#include "pch.h"
#include "OptPath.h"
#include "V2.h"
#include "MemFailExit.h"
#include <malloc.h>

static const size_t PATH_CAPACITY_MULTIPLIER = 2;

OptPath * OptPath_New(size_t capacity)
{
    OptPath * optPath = (OptPath *)malloc(sizeof(OptPath) + capacity * sizeof(V2));
    MEM_FAIL_EXIT(optPath);

    optPath->capacity = capacity;
    optPath->count = 0;

    return optPath;
}

OptPath * OptPath_Append(OptPath * optPath, V2 v2)
{
    OptPath * result = optPath;
    if (optPath->count == optPath->capacity)
    {
        size_t nextCapacity = PATH_CAPACITY_MULTIPLIER * optPath->capacity;
        result = (OptPath *)realloc(optPath, sizeof(OptPath) + nextCapacity * sizeof(V2));
        MEM_FAIL_EXIT(result);
        result->capacity = nextCapacity;
    }

    result->path[result->count++] = v2;

    return result;
}

OptPath * OptPath_Free(OptPath * optPath)
{
    free(optPath);
    return NULL;
}
