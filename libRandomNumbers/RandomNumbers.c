#include "pch.h"
#include "RandomNumbersGet.h"
#include "RandomNumbersInitClose.h"
#include "WinapiConfig.h"
#include <MemFailExit.h>
#include <stdio.h>
#include <fileapi.h>
#include <minwindef.h>
#include <malloc.h>
#include <winnt.h>

#define ERR_END_OF_SOURCE "Error: reached end of source\n"
#define ERR_READ_SOURCE "Error: can't read source, code: %d\n"

const size_t MAX_BUFFER_COUNT = (size_t)(MAXDWORD / sizeof(double)) - 1;

static HANDLE _hSource;
static double * _buffer;
static size_t _bufferCount;
static size_t _bufferIndex;
static unsigned int _bufferCountMultiplier;

static inline BOOL _FillBuffer();
static inline void _ResizeBuffer();

BOOL RandomNumbers_Initialize(HANDLE hSource, size_t baseBufferCount, unsigned int bufferCountMultiplier)
{
    _hSource = hSource;
    _bufferCount = baseBufferCount;
    _bufferCountMultiplier = bufferCountMultiplier;
    _bufferIndex = 0;

    _buffer = (double *)malloc(_bufferCount * sizeof(double));
    MEM_FAIL_EXIT(_buffer);

    return _FillBuffer();
}

int RandomNumbers_Get(double * restrict const result)
{
    if (_bufferIndex < _bufferCount)
    {
        *result = _buffer[_bufferIndex++];
        return TRUE;
    }
    else
    {
        _bufferIndex = 0;
        if (_bufferCount < MAX_BUFFER_COUNT)
            _ResizeBuffer();

        if (_FillBuffer())
        {
            *result = _buffer[_bufferIndex++];
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

void RandomNumbers_Close()
{
    if (_buffer != NULL)
        free(_buffer);
}

static inline BOOL _FillBuffer()
{
    DWORD dwNumberOfBytesRead;
    BOOL bReadResult = ReadFile(_hSource, _buffer,
                                _bufferCount * sizeof(double), &dwNumberOfBytesRead,
                                NULL);
    if (bReadResult && dwNumberOfBytesRead != 0)
    {
        return TRUE;
    }
    else if (bReadResult && dwNumberOfBytesRead == 0)
    {
        free(_buffer); _buffer = NULL;
        fprintf(stderr, ERR_END_OF_SOURCE);
        return FALSE;
    }
    else
    {
        free(_buffer); _buffer = NULL;
        fprintf(stderr, ERR_READ_SOURCE, GetLastError());
        return FALSE;
    }
}

static inline void _ResizeBuffer()
{
    _bufferCount *= _bufferCountMultiplier;
    if (_bufferCount > MAX_BUFFER_COUNT)
        _bufferCount = MAX_BUFFER_COUNT;

    free(_buffer);
    _buffer = (double *)malloc(_bufferCount * sizeof(double));
    MEM_FAIL_EXIT(_buffer);
}
