#pragma once

#include "WinapiConfig.h"

BOOL RandomNumbers_Initialize(HANDLE hSource,
                              size_t baseBufferCount,
                              unsigned int bufferCountMultiplier);
void RandomNumbers_Close();
