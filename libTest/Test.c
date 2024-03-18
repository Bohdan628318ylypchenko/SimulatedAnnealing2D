#include "pch.h"
#include "Test.h"
#include <stdio.h>

void Test_Run(const Test * restrict const arrTests)
{
    for (Test * test = arrTests; test->name != NULL && test->test != NULL; test++)
    {
        if (test->test())
            printf("PASSED %s\n", test->name);
        else
            printf("FAILED %s\n", test->name);
    }
}