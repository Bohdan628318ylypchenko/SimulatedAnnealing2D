#include "pch.h"
#include "WinapiConfig.h"
#include "MemFailExit.h"
#include "V2.h"
#include "Line.h"
#include "Polygon.h"
#include "OptPath.h"
#include "OptSA.h"
#include "RandomNumbersInitClose.h"
#include "RandomNumbersGet.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

#define USAGE "exeOptSARunner [s]rnd-source s | [x]start f | [y]start f |" \
              "[a]t0 f | [b]tmin f | [c]lmax f | [d]lmin f | [n] ull | [o]output s"

#define CONTROL_STR "sourcePath = %s; x = %Lf; y = %Lf; t0 = %Lf; tmin = %Lf; lmax = %Lf; lmin = %Lf; N = %lld; " \
                    "e1 = %Lf, e2 = %Lf; outputPath = %s\n"

#define ERR_UNKNOWN_FLAG "Error: unknown flag: %c\n"
#define ERR_NO_TEST_RND_SOUCRE "Error: no rnd source '%s' present in current dir, code = %d\n"
#define ERR_RND_INIT "Error: can't initialize rnd source, code = %d\n"
#define ERR_COUNT_WRITE "Error: can't write count to output, code = %d\n"
#define ERR_PATH_WRITE "Error: can't write path to output, code = %d\n"

static const size_t BASE_BUFFER_COUNT = 2048;
static const unsigned int BUFFER_COUNT_MULTIPLIER = 2;
static const double E1 = 0.001;
static const double E2 = 0.01;
static const double a = 1;
static const double b = 100;

double target(V2 v2)
{
    return pow(a - v2.x, 2.0) + b * pow(v2.y - pow(v2.x, 2.0), 2.0);
}

double changeTemperature(double t0, double tmin, double tcurrent)
{
    return tcurrent * 0.9;
}

double changeStepLength(double lmin, double lmax, double lcurrent,
                        unsigned long long N, unsigned long long i)
{
    double d = (lmax - lmin) / (double)N;
    double lnext = lcurrent - d;
    return lnext > lmin ? lnext : lmin;
}

int main(int argc, char *argv[])
{
    if (argc != 19)
    {
        puts(USAGE);
        return 1;
    }

    char * sourcePath = NULL;
    char * outputPath = NULL;
    double x, y, t0, tmin, lmax, lmin;
    unsigned long long N;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '/')
        {
            char flag = argv[i][1];
            switch (flag)
            {
                case 's':
                    sourcePath = argv[i + 1];
                    break;
                case 'x':
                    x = atof(argv[i + 1]);
                    break;
                case 'y':
                    y = atof(argv[i + 1]);
                    break;
                case 'a':
                    t0 = atof(argv[i + 1]);
                    break;
                case 'b':
                    tmin = atof(argv[i + 1]);
                    break;
                case 'c':
                    lmax = atof(argv[i + 1]);
                    break;
                case 'd':
                    lmin = atof(argv[i + 1]);
                    break;
                case 'n':
                    N = atoll(argv[i + 1]);
                    break;
                case 'o':
                    outputPath = argv[i + 1];
                    break;
                default:
                    fprintf(stderr, ERR_UNKNOWN_FLAG, flag);
                    puts(USAGE);
                    return 1;
            }
        }
    }

    HANDLE hSource = CreateFileA(sourcePath,
                                 GENERIC_READ, 0, NULL,
                                 OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN,
                                 NULL);
    if (hSource == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, ERR_NO_TEST_RND_SOUCRE, sourcePath, GetLastError());
        return 1;
    }

    if (!RandomNumbers_Initialize(hSource, BASE_BUFFER_COUNT, BUFFER_COUNT_MULTIPLIER))
    {
        fprintf(stderr, ERR_RND_INIT, GetLastError());
        CloseHandle(hSource);
        return 1;
    }

    HANDLE hOutput = CreateFileA(outputPath,
                                 GENERIC_WRITE, 0, NULL,
                                 CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN,
                                 NULL);
    if (hSource == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, ERR_NO_TEST_RND_SOUCRE, sourcePath, GetLastError());
        CloseHandle(hSource);
        RandomNumbers_Close();
        return 1;
    }

    printf(CONTROL_STR, sourcePath, x, y, t0, tmin, lmax, lmin, N, E1, E2, outputPath);

    V2 v2Initial = { .x = x, .y = y };
    Line g1 = { .a = 0, .b = -1, .c = -0.35 };
    Polygon * polygon = Polygon_New(1);
    polygon->lines[0] = g1;

    OptPath * optPath = OptSA(target, v2Initial, polygon, E1, E2,
                              t0, tmin, changeTemperature,
                              lmin, lmax, changeStepLength,
                              N);

    DWORD nBytesWritten; DWORD nBytesToWrite = sizeof(size_t);
    if (!WriteFile(hOutput, &(optPath->count), nBytesToWrite, &nBytesWritten, NULL) ||
        nBytesWritten != nBytesToWrite)
    {
        fprintf(stderr, ERR_COUNT_WRITE, GetLastError());
        CloseHandle(hSource);
        RandomNumbers_Close();
        CloseHandle(hOutput);
        return 1;
    }
    nBytesToWrite = 2 * sizeof(double);
    for (size_t i = 0; i < optPath->count; i++)
    {
        double buf[2];
        buf[0] = optPath->path[i].x;
        buf[1] = optPath->path[i].y;
        if (!WriteFile(hOutput, buf, nBytesToWrite, &nBytesWritten, NULL) ||
            nBytesWritten != nBytesToWrite)
        {
            fprintf(stderr, ERR_PATH_WRITE, GetLastError());
            CloseHandle(hSource);
            RandomNumbers_Close();
            CloseHandle(hOutput);
            return 1;
        }
    }

    polygon = Polygon_Free(polygon);
    optPath = OptPath_Free(optPath);
    RandomNumbers_Close();
    CloseHandle(hSource);
    CloseHandle(hOutput);

    return 0;
}