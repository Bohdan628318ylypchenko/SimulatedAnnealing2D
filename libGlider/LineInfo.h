#pragma once

#include "V2.h"

typedef struct
{
    int index;
    double distanceByDirection;
    V2 intersection;
} LineInfo;

typedef struct
{
    int capacity;
    int count;
    LineInfo info[];
} ListLineInfo;

ListLineInfo * ListLineInfo_New(int capacity);

int ListLineInfo_Append(ListLineInfo * restrict const lli, LineInfo li);

void ListLineInfo_Clear(ListLineInfo * restrict const lli);

ListLineInfo * ListLineInfo_Free(ListLineInfo * lli);
