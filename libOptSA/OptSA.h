#pragma once

#include "OptPath.h"
#include "V2.h"
#include "Polygon.h"

OptPath * OptSA(double(*targetFunction)(V2), V2 initialV2,
                const Polygon * restrict const polygon, double e1, double e2,
                double t0, double tmin,
                double(*changeTemperature)(double /*t0*/, double /*tmin*/, double /*tcurrent*/),
                double lmin, double lmax,
                double(*changeStepLength)(double /*lmin*/, double /*lmax*/, double /*lcurrent*/,
                                          unsigned long long /*N*/, unsigned long long /*i*/),
                unsigned long long N);
