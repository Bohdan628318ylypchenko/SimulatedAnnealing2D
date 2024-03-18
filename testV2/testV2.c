#include "pch.h"
#include "Eq.h"
#include "Test.h"
#include "V2.h"
#include <math.h>

int V2_DistanceTest(void) {
    V2 a = { .x = 1, .y = 1 };
    V2 b = { .x = 4, .y = 5 };

    long double result = V2_Distance(a, b);

    return result == 5;
}

int V2_NormalizeTest(void) {
    V2 a = { .x = 3, .y = 4 };
    V2 result = V2_Normalize(a);

    return Eq(sqrtl(result.x * result.x + result.y * result.y), 1.0, 0.001);
}

int main(void) {
    Test arrTests[] = {
        { .name = "V2_DistanceTest", .test = V2_DistanceTest },
        { .name = "V2_NormalizeTest", .test = V2_NormalizeTest },
        NULL
    };

    Test_Run(arrTests);

    return 0;
}

