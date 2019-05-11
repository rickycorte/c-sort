#include "sortlibs/sort.h"
#include "tests/test.h"

#include <stdio.h>


#define BENCH_TIMES 100
#define TEST_FILE_NAME "test_data.txt"


int success = 0, fails = 0;

testCase *tests;
int tests_size;


typedef struct
{
    int (*fuction)(int*, int);
    const char * name;
} sort_algorithm;


sort_algorithm algorithms[] = {

    { merge_sort, "Merge Sort" },
    { insertion_sort, "Insertion Sort" }

};

const int algorithm_count = 2;



int main(int argc, char *argv[])
{

    tests = loadTests(TEST_FILE_NAME, &tests_size);

    for(int itr =0; itr < algorithm_count; itr++)
    {

        printf("%s:", algorithms[itr].name);

        for(int i = 0; i < tests_size; i++)
        {
            printf("\n  * Running test %i: ", i);
            if(check_arr(insertion_sort, tests[i].input, tests[i].expected_output , tests[i].size))
            {
                printf("OK\n",i);
                success++;
                //printf("Running benchmark, please wait...\n");
                benckmark(merge_sort, tests[i].input, tests[i].expected_output , tests[i].size, BENCH_TIMES);

            }
            else
            {
                printf("\n     Result: ERROR\n",i);
                fails++;
            }
        }

        printf("\n\n  ! FINAL RESULT: %s\n", (success == tests_size)? "SUCCESS" : "FAIL");
        printf("    In %i tests: %i successes and %i fails\n\n", tests_size, success, fails);
    }

    clearTests(tests, tests_size);

    return 0;
}