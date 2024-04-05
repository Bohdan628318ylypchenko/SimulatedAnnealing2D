#include "pch.h"
#include "V2.h"
#include "Eq.h"
#include <math.h>

double V2_Distance(V2 a, V2 b)
{
	double lx = b.x - a.x;
	double ly = b.y - a.y;

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

V2 V2_Multiply(V2 a, double k)
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
	double m = sqrtl(a.x * a.x + a.y * a.y);

	V2 result = { .x = a.x / m, .y = a.y / m };

	return result;
}

int V2_Eq(V2 a, V2 b, double e)
{
	return Eq(a.x, b.x, e) && Eq(a.y, b.y, e);
}
