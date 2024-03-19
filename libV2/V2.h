#pragma once

typedef struct
{
    long double x;
    long double y;
} V2;

long double V2_Distance(V2 a, V2 b);

V2 V2_Add(V2 a, V2 b);

V2 V2_Multiply(V2 a, long double k);

V2 V2_DirectionAB(V2 a, V2 b);

V2 V2_Normalize(V2 a);
