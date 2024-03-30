#include "pch.h"
#include "Eq.h"
#include <math.h>

int Eq(double a, double b, double e)
{
    return fabsl(a - b) < e;
}