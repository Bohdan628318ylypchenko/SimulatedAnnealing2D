#include "pch.h"
#include <windows.h>
#include "MemFailExit.h"
#include "RandomNumbers.h"
#include "Eq.h"
#include <malloc.h>
#include <stdio.h>
#include <handleapi.h>
#include <fileapi.h>
#include <stdlib.h>

#define TEMP_FILENAME "temp"
#define NUMBER_COUNT ((MAXDWORD) / sizeof(double) - 1)
#define SEED 450

#define BASE_BUFFER_SIZE 100
#define BUFFER_SIZE_MULTIPLIER 2

#define EPSILON 0.0001

int main(void)
{
    // Create array of random numbers;
    srand(SEED);
    double * numbers = (double *)malloc(NUMBER_COUNT * sizeof(double));
    MEM_FAIL_EXIT(numbers);
    for (size_t i = 0; i < NUMBER_COUNT; i++)
    {
        numbers[i] = (double)rand() / (double)RAND_MAX;
    }

    printf("Generated %llu numbers\n", (unsigned long long)NUMBER_COUNT);

    // Open temp file
    HANDLE hFile = CreateFileA(TEMP_FILENAME,
                               GENERIC_READ | GENERIC_WRITE,
                               0,
                               NULL,
                               CREATE_ALWAYS,
                               FILE_FLAG_DELETE_ON_CLOSE | FILE_FLAG_SEQUENTIAL_SCAN,
                               NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Error: can't open temp file, code = %d\n", GetLastError());
        free(numbers);
        return 1;
    }

    // Write generated numbers to temp file
    DWORD dwNumberOfWrittenBytes; DWORD dwNumberOfBytesToWrite = NUMBER_COUNT * sizeof(double);
    BOOL bWriteResult = WriteFile(hFile, numbers, dwNumberOfBytesToWrite, &dwNumberOfWrittenBytes, NULL);
    BOOL bFlushResult = FlushFileBuffers(hFile);
    if (!bWriteResult || !bFlushResult || dwNumberOfWrittenBytes != dwNumberOfBytesToWrite)
    {
        fprintf(stderr, "Error: can't write to temp file, code = %d\n", GetLastError());
        CloseHandle(hFile);
        free(numbers);
        return 1;
    }

    // Reset file position to beginning
    if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        fprintf(stderr, "Error: can't move temp file pos to beginning, code = %d\n", GetLastError());
        CloseHandle(hFile);
        free(numbers);
        return 1;
    }

    // Initialize random numbers lib
    if (!RandomNumbers_Initialize(hFile, BASE_BUFFER_SIZE, BUFFER_SIZE_MULTIPLIER))
    {
        fprintf(stderr, "Error: can't initialize random numbers\n");
        CloseHandle(hFile);
        free(numbers);
        return 1;
    }

    // Get and assert random numbers
    for (size_t i = 0; i < NUMBER_COUNT; i++)
    {
        // Get
        double actual;
        if (!RandomNumbers_Get(&actual))
        {
            fprintf(stderr, "Error: can't get random numbers\n");
            CloseHandle(hFile);
            free(numbers);
            return 1;
        }

        // Assert
        if (!Eq(numbers[i], actual, EPSILON))
        {
            fprintf(stderr, "Error: numbers are not equal, index = %zu:\n"
                    "    expected = %Lf; actual = %Lf\n", i, numbers[i], actual);
            CloseHandle(hFile);
            free(numbers);
            return 1;
        }
    }

    puts("Test passed!");

    free(numbers);
    CloseHandle(hFile);

    return 0;
}