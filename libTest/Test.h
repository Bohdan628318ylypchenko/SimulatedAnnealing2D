#pragma once

typedef struct
{
    const char * const name;
    int (*test)(void);
} Test;

void Test_Run(const Test * restrict const arrTests);

