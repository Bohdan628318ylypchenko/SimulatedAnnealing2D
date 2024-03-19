#include "pch.h"
#include <Test.h>
#include <Eq.h>

int Eq_EqualTest(void)
{
    long double a = 5.0;
    long double b = 5.0001;
    return Eq(a, b, 0.001);
}

int Eq_NotEqualTest(void)
{
    long double a = 5.0;
    long double b = 5.0001;
    return Eq(a, b, 0.0001);
}

int main(void)
{
    Test arrTests[] = {
        {.name = "Eq_EqualTest", .test = Eq_EqualTest },
        {.name = "Eq_NotEqualTest", .test = Eq_NotEqualTest }
    };

    Test_Run(arrTests);

    return 0;
}