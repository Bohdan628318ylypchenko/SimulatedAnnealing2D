#include "pch.h"
#include "LineInfo.h"
#include "MemFailExit.h"
#include <malloc.h>

ListLineInfo * ListLineInfo_New(int capacity)
{
    ListLineInfo * lli = (ListLineInfo *)malloc(sizeof(ListLineInfo) + capacity * sizeof(LineInfo));
    MEM_FAIL_EXIT(lli);

    lli->capacity = capacity;
    lli->count = 0;

    return lli;
}

int ListLineInfo_Append(ListLineInfo * restrict const lli, LineInfo li)
{
    if (lli->capacity == lli->count)
        return 0;
    
    lli->info[lli->count++] = li;
    return 1;
}

ListLineInfo * ListLineInfo_Free(ListLineInfo * lli)
{
    free(lli);
    return NULL;
}
