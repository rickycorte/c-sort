#ifndef __C_TEST__
#define __C_TEST__

/**
 * Struttura dati un test
 */
typedef struct 
{
    int * input;
    int * expected_output;
    int size;
} testCase;



testCase *loadTests(const char file_name[], int *sz);



void clearTests(testCase * tests);

#endif