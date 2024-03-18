#pragma once

typedef struct
{
    long double x;
    long double y;
} V2;

long double V2_Distance(V2 a, V2 b);

V2 V2_Normalize(V2 a);
