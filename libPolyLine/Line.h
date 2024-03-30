#pragma once

#include "V2.h"

typedef struct
{
    double a;
    double b;
    double c;
} Line;

double Line_Evaluate(Line l, V2 v2);

Line Line_Normalize(Line l);

V2 Line_IntersectionLinePointDirection(Line l, V2 point, V2 direction);

V2 Line_PointProjectionOnNormalizedLine(Line l, V2 point);
