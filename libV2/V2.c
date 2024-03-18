#include "pch.h"
#include "V2.h"
#include <math.h>

long double V2_Distance(V2 a, V2 b)
{
	long double lx = b.x - a.x;
	long double ly = b.y - a.y;

	return sqrtl(lx * lx + ly * ly);
}

V2 V2_Normalize(V2 a)
{
	long double m = sqrtl(a.x * a.x + a.y * a.y);

	V2 result = { .x = a.x / m, .y = a.y / m };

	return result;
}
