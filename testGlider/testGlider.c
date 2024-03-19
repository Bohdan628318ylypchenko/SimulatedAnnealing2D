#include "pch.h"
#include <Test.h>
#include "Line.h"
#include <V2.h>
#include <Polygon.h>
#include <Glide.h>
#include <stdio.h>
#include <Eq.h>

static Line g1 = { .a = 1, .b = 0, .c = -6 };
static Line g2 = { .a = 1, .b = 1, .c = -13 };
static Line g3 = { .a = -1, .b = 1, .c = -3 };

static V2 a = { .x = 4, .y = 2 };
static V2 b = { .x = 12, .y = 10 };
static V2 direction;

static Polygon * polygon;

static const double e1 = 0.01;
static const double e2 = 0.01;

static const double E = 0.001;

void init(void)
{
    g1 = Line_Normalize(g1);
    g2 = Line_Normalize(g2);
    g3 = Line_Normalize(g3);

    direction = V2_Normalize(V2_DirectionAB(a, b));
}

int Glide_GlideTest1(void)
{
    polygon = Polygon_New(1);
    polygon->lines[0] = g1;

    V2 result = Glide(a, b, direction, polygon, e1, e2);

    polygon = Polygon_Free(polygon);

    printf("    result: x = %Lf; y = %Lf\n", result.x, result.y);
    return Eq(result.x, 6, E) && Eq(result.y, 10, E);
}

int Glide_GlideTest2(void)
{
    polygon = Polygon_New(2);
    polygon->lines[0] = g1;
    polygon->lines[1] = g2;

    V2 result = Glide(a, b, direction, polygon, e1, e2);

    polygon = Polygon_Free(polygon);

    printf("    result: x = %Lf; y = %Lf\n", result.x, result.y);
    return Eq(result.x, 4.5, E) && Eq(result.y, 8.5, E);
}

int Glide_GlideTest3(void)
{
    polygon = Polygon_New(3);
    polygon->lines[0] = g1;
    polygon->lines[1] = g2;
    polygon->lines[2] = g3;

    V2 result = Glide(a, b, direction, polygon, e1, e2);

    polygon = Polygon_Free(polygon);

    printf("    result: x = %Lf; y = %Lf\n", result.x, result.y);
    return Eq(result.x, 5, E) && Eq(result.y, 8, E);
}

int main(void)
{
    Test arrTests[] =
    {
        { .name = "Glide_GlideTest1", .test = Glide_GlideTest1 },
        { .name = "Glide_GlideTest2", .test = Glide_GlideTest2 },
        { .name = "Glide_GlideTest3", .test = Glide_GlideTest3 },
        NULL
    };

    init();
    
    Test_Run(arrTests);

    return 0;
}
