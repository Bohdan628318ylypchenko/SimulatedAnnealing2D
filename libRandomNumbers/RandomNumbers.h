#pragma once

#include <windows.h>

BOOL RandomNumbers_Initialize(HANDLE hSource,
                              size_t baseBufferCount,
                              unsigned int bufferCountMultiplier);

BOOL RandomNumbers_Get(double * restrict const result);
