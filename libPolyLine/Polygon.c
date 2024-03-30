#include "pch.h"
#include "Polygon.h"
#include "MemFailExit.h"
#include "Line.h"
#include <malloc.h>

Polygon * Polygon_New(int count)
{
    Polygon * p = (Polygon *)malloc(sizeof(Polygon *) + count * sizeof(Line));
    MEM_FAIL_EXIT(p);

    p->count = count;

    return p;
}

Polygon * Polygon_Free(Polygon * p)
{
    free(p);
    return NULL;
}
