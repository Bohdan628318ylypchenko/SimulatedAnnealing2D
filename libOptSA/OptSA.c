#include "pch.h"
#include "OptSA.h"
#include "OptPath.h"
#include "Glide.h"
#include "Polygon.h"
#include "V2.h"
#include "RandomNumbersGet.h"
#include <stdio.h>
#include <math.h>

#define ERR_RND "Error: can't get random number from source\n"

static inline int _GenerateDirectionAngle(double * restrict const direction);
static inline int _Trial(double targetFunctionDifference, double t);

OptPath * OptSA(double(*targetFunction)(V2), V2 initialV2,
                const Polygon * restrict const polygon, double e1, double e2,
                double t0, double tmin,
                double(*changeTemperature)(double /*t0*/, double /*tmin*/, double /*tcurrent*/),
                double lmin, double lmax,
                double(*changeStepLength)(double /*lmin*/, double /*lmax*/, double /*lcurrent*/,
                                          unsigned long long /*N*/, unsigned long long /*i*/),
                unsigned long long N)
{
    OptPath * optPath = OptPath_New(1);

    double tcurrent = t0;
    double lcurrent = lmax;
    V2 currentV2 = initialV2;
    while (tcurrent > tmin)
    {
        V2 directionV2, unboundedNextV2, glideNextV2;
        double directionAngle = 0;
        for (unsigned long long i = 0; i < N; i++)
        {
            lcurrent = changeStepLength(lmin, lmax, lcurrent, N, i);

            if (!_GenerateDirectionAngle(&directionAngle))
                return optPath;
            directionV2.x = lcurrent * cos(directionAngle);
            directionV2.y = lcurrent * sin(directionAngle);

            unboundedNextV2 = V2_Add(currentV2, directionV2);

            glideNextV2 = Glide(currentV2, unboundedNextV2, directionV2, polygon, e1, e2);

            double targetFunctionDifference = targetFunction(glideNextV2) - targetFunction(currentV2);
            if (targetFunctionDifference < 0 || _Trial(targetFunctionDifference, tcurrent))
            {
                currentV2 = glideNextV2;
                optPath = OptPath_Append(optPath, currentV2);
            }
        }
        tcurrent = changeTemperature(t0, tmin, tcurrent);
    }

    return optPath;
}

static inline int _GenerateDirectionAngle(double * restrict const direction)
{
    if (!RandomNumbers_Get(direction))
    {
        fprintf(stderr, ERR_RND);
        return 0;
    }
    *direction *= 2.0 * M_PI;
    return 1;
}

static inline int _Trial(double targetFunctionDifference, double t)
{
    double edge = exp(-1.0 * targetFunctionDifference / t);
    double trial;

    if (!RandomNumbers_Get(&trial))
        return 0;

    return trial <= edge;
}
