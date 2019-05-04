#include "sortlibs/sort.h"
#include "tests/test.h"

#include <stdio.h>


#define BENCH_TIMES 100
#define TEST_FILE_NAME "test_data.txt"


int success = 0, fails = 0;

testCase *tests;
int tests_size;


int main(int argc, char *argv[])
{

    tests = loadTests(TEST_FILE_NAME, &tests_size);

    for(int i = 0; i < tests_size; i++)
    {
        printf("\nRunning test %i: ", i);
        if(check_arr(merge_sort, tests[i].input, tests[i].expected_output , tests[i].size))
        {
            printf("OK\n",i);
            success++;
            //printf("Running benchmark, please wait...\n");
            benckmark(merge_sort, tests[i].input, tests[i].expected_output , tests[i].size, BENCH_TIMES);

        }
        else
        {
            printf("\nResult: ERROR\n",i);
            fails++;
        }
    }

    printf("\n\nFINAL RESULT: %s\n", (success == tests_size)? "SUCCESS" : "FAIL");
    printf("In %i tests: %i successes and %i fails\n", tests_size, success, fails);

    clearTests(tests, tests_size);

    return 0;
}