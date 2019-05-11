#include "sortlibs/sort.h"
#include "tests/test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BENCH_TIMES 100
#define TEST_FILE_NAME "test_data.txt"
#define HTML_TABLE_FILE_NAME "result.html"

int success = 0, fails = 0;

testCase *tests;
int tests_size;

typedef struct
{
    int (*fuction)(int *, int);
    const char *name;
} sort_algorithm;

// algorithms to test
sort_algorithm algorithms[] = {

    {merge_sort, "Merge Sort"},
    {insertion_sort, "Insertion Sort"},
    {bubble_sort, "Bubble Sort"}

};

const int algorithm_count = 3;


/**
 * Run tests and then benckmark algorithms
 */
void StandardBench()
{
    for (int itr = 0; itr < algorithm_count; itr++)
    {

        success = 0;
        fails = 0;

        printf("%s:", algorithms[itr].name);

        for (int i = 0; i < tests_size; i++)
        {
            printf("\n  * Running test %i: ", i);
            if (check_arr(algorithms[itr].fuction, tests[i].input, tests[i].expected_output, tests[i].size))
            {
                printf("OK\n", i);
                success++;
                benckmark(algorithms[itr].fuction, tests[i].input, tests[i].expected_output, tests[i].size, BENCH_TIMES);
            }
            else
            {
                printf("\n     Result: ERROR\n", i);
                fails++;
            }
        }

        printf("\n\n  ! FINAL RESULT (%s): %s\n", algorithms[itr].name, (success == tests_size) ? "SUCCESS" : "FAIL");
        printf("    In %i tests: %i successes and %i fails\n\n", tests_size, success, fails);
    }
}

/**
 * Run benchmarks and create a comparative table
 */
void BenckToHtmlTableFile()
{
    FILE *fd = fopen(HTML_TABLE_FILE_NAME, "w");
    if (!fd)
    {
        printf("Unable to create %s'n", HTML_TABLE_FILE_NAME);
        exit(1);
    }
    fprintf(fd, "<html><body><h3>Sort Algorithms</h3><table style=\"width:100%%; float:center\">");

    // write table header
    fprintf(fd, "<tr><th>#</th><th>Data Size</th>");
    for (int i = 0; i < algorithm_count; i++)
    {
        fprintf(fd, "<th>%s</th>", algorithms[i].name);
    }
    fprintf(fd, "</tr>");

    double *avgArr = malloc(algorithm_count * sizeof(double));

    // test algorithms and write average
    for (int i = 0; i < tests_size; i++)
    {
        fprintf(fd, "<tr><td>%d</td><td>%d</td>", i+1, tests[i].size);

        //run benckmark
        for(int itr = 0; itr < algorithm_count; itr++)
        {

            avgArr[itr] = benckmark(algorithms[itr].fuction, tests[i].input, tests[i].expected_output, tests[i].size, BENCH_TIMES);           
        }

        // find minimum
        int minIdx = 0;
        double min = 10000000;
        for(int itr = 0; itr < algorithm_count; itr++)
        {
            if(avgArr[itr] < min)
                {
                    min = avgArr[itr];
                    minIdx = itr;                   
                }
        }

        // write line to file
        for(int itr = 0; itr < algorithm_count; itr++)
        {
            fprintf(fd, "<td %s>%.3f ms</td>", (itr == minIdx) ? "style=\"background-color:#98ff91\"" : "", avgArr[itr]);
        }



        fprintf(fd, "</tr>");
    }

    fprintf(fd, "</table></body></html>");
    fclose(fd);

    printf("\nWrote comparision table to %s\n", HTML_TABLE_FILE_NAME);
}


int main(int argc, char **argv)
{

    tests = loadTests(TEST_FILE_NAME, &tests_size);

    if (argc == 2 && strcmp(argv[1], "-c") == 0)
    {
        BenckToHtmlTableFile();
    }
    else
    {
        StandardBench();
    }

    clearTests(tests, tests_size);

    return 0;
}