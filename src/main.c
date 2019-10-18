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
    {heap_sort, "Heapsort [R]"},
    {quick_sort, "Quicksort [R]"},
    {insertion_sort, "Insertion Sort"},
    {bubble_sort, "Bubble Sort"}

};

const int algorithm_count = 5;


/**
 * Write a progress bar in terminal
 * Make sure to run this function sequentially! No stoud in the middle or it will try to delete it!
 * 
 * @param value current progression value
 * @param max maximum expected value
 * @param size bar size (20 give best results)
 * @param desc bar description
 */ 
void make_progress_bar(int value, int max, int size, const char *desc)
{
    double prg_perc = ((double)value) / max;
    int progress = prg_perc * size;
    int desc_sz = strlen(desc);

    //print new bar
    // \r -> go back to line start
    printf("\r[");
    for (int i = 0; i < progress -1; i++)
    {
        printf("=");
    }
    if(progress != size)
        printf(">");
    else
        printf("=");
    for (int i = 0; i < size - progress; i++)
    {
        printf(" ");
    }

    int perc = prg_perc * 100;
    printf("] %3d%% %s", perc, desc);

    //add some white space to the line end to ovverride extra characters moved by percentual size (1/2 spaces)
    printf("    ");

    if(value == max)
        printf(": DONE\n");
    
}



/**
 * Run tests and then benckmark algorithms
 */
void StandardBench(int verbose)
{
    for (int itr = 0; itr < algorithm_count; itr++)
    {

        success = 0;
        fails = 0;

        if(verbose) 
            printf("%s:", algorithms[itr].name);

        for (int i = 0; i < tests_size; i++)
        {

            if(!verbose && fails == 0)
                make_progress_bar(i+1, tests_size, 20, algorithms[itr].name);

            if(verbose)
                printf("\n  * Running test %d: ", i);

            if (check_arr(algorithms[itr].fuction, tests[i].input, tests[i].expected_output, tests[i].size))
            {
                if(verbose) 
                    printf("OK\n");

                success++;
                benckmark(algorithms[itr].fuction, tests[i].input, tests[i].expected_output, tests[i].size, BENCH_TIMES, verbose);
            }
            else
            {
                if(verbose) 
                {
                    printf("\n     Result: ERROR\n");
                }
                else
                {
                    printf("Failed test\n");
                }
                

                fails++;
            }
        }
        
        printf("%sFINAL RESULT (%s): %s\n", (verbose)? "\n\n  ! " : "" , algorithms[itr].name, (success == tests_size) ? "SUCCESS" : "FAIL");
        printf("%sIn %d tests: %d successes and %d fails\n\n", (verbose) ? "    " : "" , tests_size, success, fails);
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
        fprintf(fd, "<tr><td style=\"text-align:center; width:50px\">%d</td><td style=\"text-align:center; width:100px\">%d</td>", i+1, tests[i].size);

        make_progress_bar(i+1, tests_size, 20, "Testing algorithms");

        //run benckmark
        for(int itr = 0; itr < algorithm_count; itr++)
        {          
            avgArr[itr] = benckmark(algorithms[itr].fuction, tests[i].input, tests[i].expected_output, tests[i].size, BENCH_TIMES, 0);           
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
            fprintf(fd, "<td style=\"text-align:center%s\">%.3f ms</td>", (itr == minIdx) ? "; background-color:#98ff91" : "", avgArr[itr]);
        }

        fprintf(fd, "</tr>");
    }

    fprintf(fd, "</table></body></html>");
    fclose(fd);
    free(avgArr);
    printf("\nWrote comparision table to %s\n", HTML_TABLE_FILE_NAME);
}


int hasChar(const char *str, char c)
{
    for(int i = 0; i < strlen(str); i++ )
    {
        if(str[i] == c) return 1;
    }

    return 0;
}


int main(int argc, char **argv)
{

    tests = loadTests(TEST_FILE_NAME, &tests_size);


    if (argc == 2 && hasChar(argv[1], 'c'))
    {
        BenckToHtmlTableFile();
    }
    else
    {
        StandardBench(argc == 2 && hasChar(argv[1], 'v'));
    }

    clearTests(tests, tests_size);

    return 0;
}
