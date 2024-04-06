#include "pch.h"
#include "OptSA.h"
#include "OptPath.h"
#include "WinapiConfig.h"
#include "Eq.h"
#include "Test.h"
#include "V2.h"
#include "Line.h"
#include "Polygon.h"
#include "RandomNumbersGet.h"
#include "RandomNumbersInitClose.h"
#include "MemFailExit.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define ERR_NO_TEST_RND_SOUCRE "Error: no rnd source '%s' present in current dir, code = %d\n"
#define ERR_RND_INIT "Error: can't initialize rnd source, code = %d\n"

static const char * RND_SOURCE_FILENAME = "./testRndSource.rn";
//static const char * RND_SOURCE_FILENAME = "J:\\repos\\RandomNumberDatasets\\testRndSource.rn";
static const size_t baseBufferCount = 1000;
static const unsigned int bufferCountMultiplier = 2;

static const double E = 0.05;

static HANDLE hSource;

static int beforeAll()
{
    hSource = CreateFileA(RND_SOURCE_FILENAME,
                          GENERIC_READ, 0, NULL,
                          OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN,
                          NULL);
    if (hSource == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, ERR_NO_TEST_RND_SOUCRE, RND_SOURCE_FILENAME, GetLastError());
        return 0;
    }

    if (!RandomNumbers_Initialize(hSource, baseBufferCount, bufferCountMultiplier))
    {
        fprintf(stderr, ERR_RND_INIT, GetLastError());
        CloseHandle(hSource);
        return 0;
    }

    return 1;
}

static void afterAll()
{
    RandomNumbers_Close();
    CloseHandle(hSource);
}

double changeTemperature1(double t0, double tmin, double tcurrent)
{
    return (-1.0) / (tcurrent - 1001.0);
}

double changeStepLength1(double lmin, double lmax, double lcurrent,
                         unsigned long long N, unsigned long long i)
{
    double d = (lmax - lmin) / (double)N;
    double lnext = lcurrent - d;
    return lnext > lmin ? lnext : lmin;
}

double quadratic1(V2 v2)
{
    return (v2.x - 4) * (v2.x - 4) + (v2.y - 3) * (v2.y - 3);
}

int OptSA_Quadratic1MinInBoundTest(void)
{
    V2 initialV2 = { .x = 2.0, .y = 3.0 };

    Line g1 = { .a = -0.5, .b = -0.2, .c = 1.0   };
    Line g2 = { .a = 0.5,  .b = -1,   .c = 0.0   };
    Line g3 = { .a = 0.0,  .b = 1.0,  .c = -4.0  };
    Polygon * polygon = Polygon_New(3);
    polygon->lines[0] = g1;
    polygon->lines[1] = g2;
    polygon->lines[2] = g3;

    double e1 = 0.001;
    double e2 = 0.01;
    double t0, tmin;
    t0 = 5.0; tmin = 0.001;
    double lmin, lmax;
    lmin = 0.003; lmax = 0.5;
    unsigned long long N = 1000;

    OptPath * optPath = OptSA(quadratic1, initialV2, polygon, e1, e2,
                              t0, tmin, changeTemperature1,
                              lmin, lmax, changeStepLength1,
                              N);

    V2 actualV2 = optPath->path[optPath->count - 1];
    for (int i = 0; i < optPath->count; i++)
        printf("    result %d: x = %f; y = %f\n", i, optPath->path[i].x, optPath->path[i].y);

    V2 expectedV2 = { .x = 4.0, .y = 3.0 };
    int result = V2_Eq(actualV2, expectedV2, E);

    polygon = Polygon_Free(polygon);
    optPath = OptPath_Free(optPath);

    return result;
}


int main(void)
{
    if (!beforeAll())
        return 1;
    Test arrTests[] =
    {
        { .name = "OptSA_Quadratic1MinInBoundTest", .test = OptSA_Quadratic1MinInBoundTest },
        NULL
    };

    Test_Run(arrTests);

    afterAll();

    return 0;
}