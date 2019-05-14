#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* ----------------------------------------------------------------------- */

// ns() function to get benchmark time with high precision

#include <stdint.h>
#if defined(__linux)
#  define HAVE_POSIX_TIMER
#  include <time.h>
#  ifdef CLOCK_MONOTONIC
#     define CLOCKID CLOCK_MONOTONIC
#  else
#     define CLOCKID CLOCK_REALTIME
#  endif
#elif defined(__APPLE__)
#  define HAVE_MACH_TIMER
#  include <mach/mach_time.h>
#elif defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif
static uint64_t ns() {
  static uint64_t is_init = 0;
#if defined(__APPLE__)
    static mach_timebase_info_data_t info;
    if (0 == is_init) {
      mach_timebase_info(&info);
      is_init = 1;
    }
    uint64_t now;
    now = mach_absolute_time();
    now *= info.numer;
    now /= info.denom;
    return now;
#elif defined(__linux)
    static struct timespec linux_rate;
    if (0 == is_init) {
      clock_getres(CLOCKID, &linux_rate);
      is_init = 1;
    }
    uint64_t now;
    struct timespec spec;
    clock_gettime(CLOCKID, &spec);
    now = spec.tv_sec * 1.0e9 + spec.tv_nsec;
    return now;
#elif defined(_WIN32)
    static LARGE_INTEGER win_frequency;
    if (0 == is_init) {
      QueryPerformanceFrequency(&win_frequency);
      is_init = 1;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (uint64_t) ((1e9 * now.QuadPart)  / win_frequency.QuadPart);
#endif
}
/* ----------------------------------------------------------------------- */



/**
 * Bubble sort implementation to generate SAFE sorted output for tests
 * We use this function to sort arrays loaded from test_data.txt
 * 
 * Why bubble sort?
 * Because it is a really simple and well known implementation hard code in the wrong way!
 * We can consider an array sorted with this function "a safe result" to use to check other implementations.
 */

static int * bubble_sort(int *arr, int size)
{
    int *res = malloc(size * sizeof(int));
    memcpy(res, arr, size * sizeof(int));
    int temp;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < i; j++)
        {
            if(res[j] > res[i])
            {
                temp = res[j];
                res[j] = res[i];
                res[i] = temp;
            } 
        }
    }

    return res;
}


/**
 * print array in terminal
 * 
 * @param arr array to print
 * @param size arr size
 * @param limit max printed values
 */
static void printArr(int * arr, int size, int limit)
{
    for(int i = 0; i < size && i < limit; i++)
    {
        printf("%d, ", arr[i]);
    }

    if(size > limit)
        printf("...");
}


//TODO: migliorare la gestione degli errori
testCase *loadTests(const char file_name[], int *size)
{
    FILE *fp;

    fp = fopen(file_name, "r");
    if(!fp)
    {
        printf("Unable to find file: %s\n", file_name);
        return NULL;
    }

    char c;
    char num_buffer[10];
    int used_buffer = 0;

    int *arr = NULL, arr_size = 0, arr_allocated_size = 0;
    int test_allocated_size = 0;
    testCase *tests = NULL;

    memset(num_buffer, 0, 10); // set number buffer to 0

    while( (c = fgetc(fp)) != EOF)
    {

        // number found
        if(c == ' ' || c == '\n')
        {
            int num = atoi(num_buffer);
            memset(num_buffer, 0, 10); // set number buffer to 0
            used_buffer = 0;

            // expand allocated array, or allocate a new one
            if(arr_size >= arr_allocated_size)
            {               
                arr_allocated_size += 5;
                arr = realloc(arr, arr_allocated_size * sizeof(int));             
            }

            // add read number to array
            arr[arr_size] = num;
            arr_size++;
        }

        // found array end
        if(c == '\n')
        {
            if(arr_size < 1) 
            {
                printf("Format error, arrays must have at leat one element!\n");
                continue;
            }

            // extend tests array if more space is required
            if(*size >= test_allocated_size)
            {
                test_allocated_size += 5;
                tests = realloc(tests, test_allocated_size * sizeof(testCase));
            }    

            // create the new test
            tests[*size].input = arr;
            tests[*size].expected_output = bubble_sort(arr, arr_size);
            tests[*size].size = arr_size;

            (*size)++;

            // reset values for the next test element
            arr = NULL;
            arr_allocated_size = 0;
            arr_size = 0;
            continue;   
        }

        // add char to number string represantatio
        if(c != ' ' && c != '\n')
        {
            num_buffer[used_buffer] = c;
            used_buffer++;
        }

        // range check, not really super safe but still better than nothing
        if(used_buffer >= 9) 
        {
            printf("Format error! Cant accept numbers bigger than 2 * 10^9\n");
            return NULL;
        }

    }


    fclose(fp);
    return tests;
}


void clearTests(testCase * tests, int size)
{
    for(int i = 0; i < size; i++)
    {
        free(tests[i].input);
        free(tests[i].expected_output);
    }

    free(tests);
}



int check_arr(int (*sorter)(int*, int), int * in, int * output, int size)
{
    // copy input array, at least we can reuse the test

    int * input = malloc(size * sizeof(int));

    // no check, the program should crash here if we don't have enouth ram

    memcpy(input, in, size * sizeof(int));  

    // run sort algorithm
    sorter(input, size);


    // compare sort result with expected result
    for(int i = 0; i < size; i++)
    {

        if(input[i] != output[i])
        {
            free(input);
            // print info for a failed test
            printf("\n > Input: "); printArr(in, size, 5);
            printf("\n > Got: "); printArr(input, size, 5);
            printf("\n > Expected: "); printArr(output, size, 5);
            printf("\n");
            return 0;
        }
    }

    free(input);
    return 1;
}


double benckmark(int (*sorter)(int*, int), int * in, int * output, int size, int exec_times, int verbose)
{
    double delta_tm, start_tm, best_tm, worst_tm, sum_tm = 0;

    double avg;

    // we run the benchmark multiple times to reduce random performance spikes
    for(int i = 0; i < exec_times; i++)
    {
        start_tm = ns();
        check_arr(sorter, in, output, size);
        delta_tm = ns() - start_tm;

        // calculate best/worst run time
        if(i == 0)
        {
            best_tm = delta_tm;
            worst_tm = delta_tm;
        }
        else
        {
            if(delta_tm < best_tm) best_tm = delta_tm;

            if(delta_tm > worst_tm ) worst_tm = delta_tm;
        }
        
        sum_tm += delta_tm;
    }
    
    avg = sum_tm / 1000 / exec_times;

    if(verbose)
        printf("    BENCHMARK (size: %d)> Avg: %.2fms | Best: %.2fms | Worst: %.2fms", size, avg, best_tm/1000, worst_tm/1000); 
    
    return avg;

}