#include "pch.h"
#include "V2.h"
#include <math.h>

long double V2_Distance(V2 a, V2 b)
{
	long double lx = b.x - a.x;
	long double ly = b.y - a.y;

	return sqrtl(lx * lx + ly * ly);
}

V2 V2_Add(V2 a, V2 b)
{
	V2 result =
	{
		.x = a.x + b.x,
		.y = a.y + b.y
	};

	return result;
}

V2 V2_Multiply(V2 a, long double k)
{
	V2 result =
	{
		.x = a.x * k,
		.y = a.y * k
	};

	return result;
}

V2 V2_DirectionAB(V2 a, V2 b)
{
	V2 result =
	{
		.x = b.x - a.x,
		.y = b.y - a.y
	};

	return result;
}

V2 V2_Normalize(V2 a)
{
	long double m = sqrtl(a.x * a.x + a.y * a.y);

	V2 result = { .x = a.x / m, .y = a.y / m };

	return result;
}
