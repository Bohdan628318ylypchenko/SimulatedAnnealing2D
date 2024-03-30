#pragma once

typedef struct
{
    double x;
    double y;
} V2;

double V2_Distance(V2 a, V2 b);

V2 V2_Add(V2 a, V2 b);

V2 V2_Multiply(V2 a, double k);

V2 V2_DirectionAB(V2 a, V2 b);

V2 V2_Normalize(V2 a);
