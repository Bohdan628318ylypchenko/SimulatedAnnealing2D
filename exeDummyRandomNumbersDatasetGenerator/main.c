#include "pch.h"
#include <winnt.h>
#include <stdio.h>
#include <fileapi.h>
#include <WinBase.h>
#include <handleapi.h>
#include <stdlib.h>
#include <minwindef.h>
#include <malloc.h>
#include <MemFailExit.h>

static const size_t MAX_NUMBER_COUNT = (size_t)MAXDWORD / sizeof(double) - 1;
static const unsigned int SEED = 451;

int wmain(int argc, const wchar_t * argv[])
{
    if (argc != 4)
    {
        wprintf(L"Usage: exeDRND path-to-output-file number-count batch-count\n");
        return 1;
    }

    const wchar_t * filename = argv[1];
    const size_t number_count = _wtoll(argv[2]);
    const size_t batch_count = _wtoll(argv[3]);
    
    wprintf(L"Args: filename = %ls, number-count = %llu, batch-count = %llu\n",
            filename, number_count, batch_count);

    if (number_count == 0 || batch_count == 0)
    {
        fwprintf(stderr, L"Error: count arguments should be valid integers\n");
        return 1;
    }

    if (number_count > MAX_NUMBER_COUNT)
    {
        fwprintf(stderr, L"Error: number count can't be bigger than %llu\n", MAX_NUMBER_COUNT);
        return 1;
    }

    HANDLE hFile = CreateFileW(filename,
                               GENERIC_WRITE,
                               0,
                               NULL,
                               CREATE_ALWAYS,
                               FILE_FLAG_SEQUENTIAL_SCAN,
                               NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        fwprintf(stderr, L"Error: can't open file %ls, code = %d\n", filename, GetLastError());
        return 1;
    }

    double * batch = (double *)malloc(number_count * sizeof(double));
    MEM_FAIL_EXIT(batch);
    DWORD dwNumberOfBytesToWrite = number_count * sizeof(double); 
    srand(SEED);
    for (size_t i = 0; i < batch_count; i++)
    {
        for (size_t j = 0; j < number_count; j++)
            batch[j] = (double)rand() / (double)RAND_MAX;

        DWORD dwNumberOfBytesWritten;
        BOOL bWriteResult = WriteFile(hFile, batch, dwNumberOfBytesToWrite, &dwNumberOfBytesWritten, NULL);
        if (!bWriteResult || dwNumberOfBytesWritten != dwNumberOfBytesToWrite)
        {
            fwprintf(stderr, L"Error: can't write to file %ls, code = %d\n", filename, GetLastError());
            free(batch);
            CloseHandle(hFile);
            return 1;
        }
    }

    if (!FlushFileBuffers(hFile) || !CloseHandle(hFile))
    {
        fwprintf(stderr, L"Error: can't flush and close file %ls, code = %d\n", filename, GetLastError());
        free(batch);
        return 1;
    }

    free(batch);
    return 0;
}