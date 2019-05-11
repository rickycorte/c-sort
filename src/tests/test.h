#ifndef __C_TEST__
#define __C_TEST__

/**
 * Data structure for a test
 */
typedef struct 
{
    int * input;
    int * expected_output;
    int size;
} testCase;


/**
 * Load all integer arrays from a text file
 * 
 * @param tile_name file to open to load tests
 * @param size variable where tests size is loaded
 * 
 * @return pointer to tests array, NULL on error
 */ 
testCase *loadTests(const char file_name[], int *size);


/**
 * Remove all tests allocations
 * 
 * @param tests tests array to delete
 * @param size tests array size
 */ 
void clearTests(testCase * tests, int size);


/**
 * Check if a sort function returns the right output for an array
 * 
 * @param sorter pointer to function int <>(int *, int)
 * @param in input array to sort with sorter function
 * @param output expected result of sort operation
 * @param size input and output array sizes
 * 
 * @return 1 on success, 0 otherwise
 */
int check_arr(int (*sorter)(int*, int), int * in, int * output, int size);


/**
 * Run function benckmark and print results
 * 
 * @param sorter pointer to function int <>(int *, int)
 * @param in input array to sort with sorter function
 * @param output expected result of sort operation
 * @param size input and output array sizes
 * @param exec_times number of times to run the test
 */
void benckmark(int (*sorter)(int*, int), int * in, int * output, int size, int exec_times);

#endif