#pragma once

#include "V2.h"
#include "Polygon.h"

V2 Glide(V2 a, V2 b, V2 direction,
         const Polygon * restrict const polygon,
         long double e1, long double e2);
