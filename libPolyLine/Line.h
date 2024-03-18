#pragma once

#include "V2.h"

typedef struct
{
    long double a;
    long double b;
    long double c;
} Line;

long double Line_Evaluate(Line l, V2 v2);

Line Line_Normalize(Line l);

V2 Line_IntersectionLinePointDirection(Line l, V2 point, V2 direction);

V2 Line_PointProjectionOnNormalizedLine(Line l, V2 point);
