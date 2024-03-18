#include "pch.h"
#include <Line.h>
#include <Test.h>
#include <V2.h>
#include <math.h>

int Line_EvaluateTest(void) {
    Line l = { .a = 3, .b = 4, .c = 10 };
    V2 v2 = { .x = -1, .y = -2 };

    long double result = Line_Evaluate(l, v2);

    return fabsl(result - (-1)) < 0.001;
}

int Line_NormalizeTest(void) {
    Line l = { .a = 3, .b = 4, .c = 10 };
    Line result = Line_Normalize(l);

    return result.a == 0.6 && result.b == 0.8 && result.c == 2;
}

int Line_IntersectionLinePointDirectionTest(void) {
    Line l = { .a = 7.8, .b = 2.12, .c = 6.123 };
    V2 point = { .x = 0, .y = 0.1 };
    V2 direction = { .x = -0.01, .y = -1 };

    V2 result = Line_IntersectionLinePointDirection(l, point, direction);

    return (fabsl(result.x - (-0.028821)) < 0.001) && (fabsl(result.y - (-2.7821)) < 0.001);
}

int Line_ProjectionTest(void) {
    Line l = { .a = 2, .b = -1, .c = 3 };
    l = Line_Normalize(l);
    V2 v2 = { .x = 3, .y = 3 };

    V2 result = Line_PointProjectionOnNormalizedLine(l, v2);

    return (fabsl(result.x - 0.6) < 0.001 && fabsl(result.y - 4.2) < 0.001);
}

int main(void) {
    Test arrTests[] = {
        {.name = "Line_EvaluateTest", .test = Line_EvaluateTest },
        {.name = "Line_NormalizeTest", .test = Line_NormalizeTest },
        {.name = "Line_IntersectionLinePointDirectionTest", .test = Line_IntersectionLinePointDirectionTest },
        {.name = "Line_ProjectionTest", .test = Line_ProjectionTest },
        NULL
    };

    Test_Run(arrTests);

    return 0;
}