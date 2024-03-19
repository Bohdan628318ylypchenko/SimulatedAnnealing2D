#include "pch.h"
#include "Eq.h"
#include <math.h>

int Eq(long double a, long double b, long double e)
{
    return fabsl(a - b) < e;
}