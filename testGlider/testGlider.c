#include "pch.h"
#include <Test.h>
#include "Line.h"
#include <V2.h>
#include <Polygon.h>
#include <Glide.h>
#include <stdio.h>

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
    printf("    result: x = %Lf; y = %Lf\n", result.x, result.y);

    polygon = Polygon_Free(polygon);

    V2 expected = { .x = 6, .y = 10 };
    return V2_Eq(result, expected, E);
}

int Glide_GlideTest2(void)
{
    polygon = Polygon_New(2);
    polygon->lines[0] = g1;
    polygon->lines[1] = g2;

    V2 result = Glide(a, b, direction, polygon, e1, e2);
    printf("    result: x = %Lf; y = %Lf\n", result.x, result.y);

    polygon = Polygon_Free(polygon);

    V2 expected = { .x = 4.5, .y = 8.5 };
    return V2_Eq(result, expected, E);
}

int Glide_GlideTest3(void)
{
    polygon = Polygon_New(3);
    polygon->lines[0] = g1;
    polygon->lines[1] = g2;
    polygon->lines[2] = g3;

    V2 result = Glide(a, b, direction, polygon, e1, e2);
    printf("    result: x = %Lf; y = %Lf\n", result.x, result.y);

    polygon = Polygon_Free(polygon);

    V2 expected = { .x = 5, .y = 8 };
    return V2_Eq(result, expected, E);
}

int Glide_GlideTestAngle1(void)
{
    Line gAngle1 = { .a = 1, .b = 0, .c = -6 };
    Line gAngle2 = { .a = 1, .b = 1, .c = -11 };
    Line gAngle3 = { .a = 2, .b = 1, .c = -17 };

    polygon = Polygon_New(3);
    polygon->lines[0] = Line_Normalize(gAngle1);
    polygon->lines[1] = Line_Normalize(gAngle2);
    polygon->lines[2] = Line_Normalize(gAngle3);

    V2 aAngle = { .x = 5, .y = 3 };
    V2 bAngle = { .x = 9, .y = 11 };
    V2 directionAngle = V2_Normalize(V2_DirectionAB(aAngle, bAngle));

    V2 result = Glide(aAngle, bAngle, directionAngle, polygon, 0.001, 0.01);

    V2 expected = { .x = 4.5, .y = 6.5 };
    return V2_Eq(result, expected, E);
}

int Glide_GlideTestAngle2(void)
{
    Line gAngle1 = { .a = -1, .b = 0, .c = 0 };
    Line gAngle2 = { .a = 1, .b = -1, .c = 0 };

    polygon = Polygon_New(2);
    polygon->lines[0] = Line_Normalize(gAngle1);
    polygon->lines[1] = Line_Normalize(gAngle2);

    V2 aAngle = { .x = 1, .y = 2 };
    V2 bAngle = { .x = -0.5, .y = -1 };
    V2 directionAngle = V2_Normalize(V2_DirectionAB(aAngle, bAngle));

    V2 result = Glide(aAngle, bAngle, directionAngle, polygon, 1e-7, 1e-3);
    V2 expected = { .x = 0, .y = 0 };

    return V2_Eq(result, expected, E);
}

int main(void)
{
    Test arrTests[] =
    {
        { .name = "Glide_GlideTest1", .test = Glide_GlideTest1 },
        { .name = "Glide_GlideTest2", .test = Glide_GlideTest2 },
        { .name = "Glide_GlideTest3", .test = Glide_GlideTest3 },
        { .name = "Glide_GlideTestAngle1", .test = Glide_GlideTestAngle1 },
        { .name = "Glide_GlideTestAngle2", .test = Glide_GlideTestAngle2 },
        NULL
    };

    init();
    
    Test_Run(arrTests);

    return 0;
}
