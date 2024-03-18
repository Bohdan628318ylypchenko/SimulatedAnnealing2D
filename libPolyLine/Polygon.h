#pragma once

#include "Line.h"

typedef struct
{
    int count;
    Line lines[];
} Polygon;

Polygon * Polygon_New(int count);

Polygon * Polygon_Free(Polygon * p);
