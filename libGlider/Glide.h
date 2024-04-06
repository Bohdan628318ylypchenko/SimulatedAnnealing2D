#pragma once

#include "V2.h"
#include "Polygon.h"

V2 Glide(V2 a, V2 b, V2 direction,
         const Polygon * restrict const polygon,
         double e1, double e2);
