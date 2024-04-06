#include "pch.h"
#include "Glide.h"
#include "LineInfo.h"
#include <Polygon.h>
#include <V2.h>
#include <MemFailExit.h>
#include <Line.h>
#include <float.h>
#include <Eq.h>

static V2 _Glide(V2 a, V2 b, V2 direction,
                 const Polygon * restrict const polygon,
                 double e1, double e2,
                 ListLineInfo * restrict const unsatisfiedLinesInfo,
                 ListLineInfo * restrict const closestUnsatisfiedLinesInfo,
                 int * restrict const arrIsPassedFlag);

static inline void _FindAllUnsatisfiedLines(V2 a, V2 b, V2 direction,
                                            const Polygon * restrict const polygon,
                                            double e1,
                                            ListLineInfo * restrict const listLineInfo,
                                            const int * restrict const arrIsPassedFlag);

static inline void _FindClosestUnsatisfiedLines(const ListLineInfo * restrict const unsatisfiedLinesInfo,
                                                ListLineInfo * restrict const closestUnsatisfiedLinesInfo,
                                                double e);

V2 Glide(V2 a, V2 b, V2 direction,
         const Polygon * restrict const polygon,
         double e1, double e2)
{
    ListLineInfo * unsatsfiedLinesInfo = ListLineInfo_New(polygon->count);
    ListLineInfo * closestUnsatisfiedLinesInfo = ListLineInfo_New(polygon->count);
    int * arrIsPassedFlag = (int *)calloc(polygon->count, sizeof(int));
    MEM_FAIL_EXIT(arrIsPassedFlag);

    V2 result = _Glide(a, b, direction, polygon, e1, e2,
                       unsatsfiedLinesInfo, closestUnsatisfiedLinesInfo,
                       arrIsPassedFlag);

    free(arrIsPassedFlag);
    ListLineInfo_Free(unsatsfiedLinesInfo);
    ListLineInfo_Free(closestUnsatisfiedLinesInfo);

    return result;
}

static V2 _Glide(V2 a, V2 b, V2 direction,
                 const Polygon * restrict const polygon,
                 double e1, double e2,
                 ListLineInfo * restrict const unsatisfiedLinesInfo,
                 ListLineInfo * restrict const closestUnsatisfiedLinesInfo,
                 int * restrict const arrIsPassedFlag)
{
    _FindAllUnsatisfiedLines(a, b, direction,
                             polygon, e1,
                             unsatisfiedLinesInfo, arrIsPassedFlag);

    if (unsatisfiedLinesInfo->count == 0)
        return b;

    _FindClosestUnsatisfiedLines(unsatisfiedLinesInfo, closestUnsatisfiedLinesInfo, e1);

    V2 aNext, bNext, directionNext;
    if (closestUnsatisfiedLinesInfo->count == 1)
    {
        LineInfo finalLineInfo = closestUnsatisfiedLinesInfo->info[0];
        Line finalLine = polygon->lines[finalLineInfo.index];

        arrIsPassedFlag[finalLineInfo.index] = 1;
        aNext = Line_IntersectionLinePointDirection(finalLine, a, direction);
        bNext = Line_PointProjectionOnNormalizedLine(finalLine, b);
        directionNext = V2_Normalize(V2_DirectionAB(aNext, bNext));
    }
    else
    {
        for (int i = 0; i < closestUnsatisfiedLinesInfo->count; i++)
        {
            aNext = closestUnsatisfiedLinesInfo->info[i].intersection;
            bNext = Line_PointProjectionOnNormalizedLine(polygon->lines[closestUnsatisfiedLinesInfo->info[i].index], b);
            directionNext = V2_Normalize(V2_DirectionAB(aNext, bNext));
            
            V2 directionTest = V2_Add(aNext, V2_Multiply(directionNext, e2));
            int directionTestInboundFlag = 1;
            for (int j = 0; j < polygon->count; j++)
            {
                if (Line_Evaluate(polygon->lines[j], directionTest) > e1)
                {
                    directionTestInboundFlag = 0;
                    break;
                }
            }

            if (directionTestInboundFlag)
                break;
        }
    }

    if (V2_Distance(aNext, bNext) >= e2)
    {
        unsatisfiedLinesInfo->count = closestUnsatisfiedLinesInfo->count = 0;
        return _Glide(aNext, bNext, directionNext, polygon, e1, e2,
                      unsatisfiedLinesInfo, closestUnsatisfiedLinesInfo,
                      arrIsPassedFlag);
    }
    else
    {
        return aNext;
    }
}

static inline void _FindAllUnsatisfiedLines(V2 a, V2 b, V2 direction,
                                            const Polygon * restrict const polygon,
                                            double e1,
                                            ListLineInfo * restrict const listLineInfo,
                                            const int * restrict const arrIsPassedFlag)
{
    for (int i = 0; i < polygon->count; i++)
    {
        if (arrIsPassedFlag[i] || (Line_Evaluate(polygon->lines[i], b) <= e1))
            continue;

        V2 intersection = Line_IntersectionLinePointDirection(polygon->lines[i], a, direction);
        double distanceByDirection = V2_Distance(a, intersection);
        LineInfo lineInfo =
        {
            .index = i,
            .distanceByDirection = distanceByDirection,
            .intersection = intersection
        };

        ListLineInfo_Append(listLineInfo, lineInfo);
    }
}

static inline void _FindClosestUnsatisfiedLines(const ListLineInfo * restrict const unsatisfiedLinesInfo,
                                                ListLineInfo * restrict const closestUnsatisfiedLinesInfo,
                                                double e)
{
    double minDistance = LDBL_MAX;
    for (int i = 0; i < unsatisfiedLinesInfo->count; i++)
        if (minDistance > unsatisfiedLinesInfo->info[i].distanceByDirection)
            minDistance = unsatisfiedLinesInfo->info[i].distanceByDirection;

    for (int i = 0; i < unsatisfiedLinesInfo->count; i++)
        if (Eq(minDistance, unsatisfiedLinesInfo->info[i].distanceByDirection, e))
            ListLineInfo_Append(closestUnsatisfiedLinesInfo, unsatisfiedLinesInfo->info[i]);
}
