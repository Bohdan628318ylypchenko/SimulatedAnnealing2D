#include "pch.h"
#include "Line.h"
#include <V2.h>
#include <math.h>

long double Line_Evaluate(Line l, V2 v2)
{
    return l.a * v2.x + l.b * v2.y + l.c;
}

Line Line_Normalize(Line l)
{
    long double m = sqrtl(l.a * l.a + l.b * l.b);

    Line result =
    {
        .a = l.a / m,
        .b = l.b / m,
        .c = l.c / m
    };

    return result;
}

V2 Line_IntersectionLinePointDirection(Line l, V2 point, V2 direction)
{
    long double t =
        (-1.0 / (l.a * direction.x + l.b * direction.y)) * (l.c + l.a * point.x + l.b * point.y);

    V2 result =
    {
        .x = point.x + t * direction.x,
        .y = point.y + t * direction.y
    };

    return result;

}

V2 Line_PointProjectionOnNormalizedLine(Line l, V2 point)
{
    long double t = -1.0 * (l.c + l.a * point.x + l.b * point.y);

    V2 result =
    {
        .x = point.x + t * l.a,
        .y = point.y + t * l.b
    };

    return result;
}