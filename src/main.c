#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_SIZE 5

typedef struct 
{
    int * input;
    int * expected_output;
    int size;
} testCase;


testCase tests[TEST_SIZE];

int a[] = {-4 -56, 1, 836, 3, 27, 9, 99999, 0, -18263, 0};
int b[] = {1,2,3};
int c[] = {10,9,8,7,6,5,4,3,2,1,0};
int d[] = {80, 43,3,1,5,763,-1};
int e[] = {1};

void createTests()
{
    tests[1].input = a;
    tests[1].expected_output= bubble_sort(a, 11);
    tests[1].size = 11;

    tests[0].input = b;
    tests[0].expected_output= bubble_sort(b, 3);
    tests[0].size = 3;

    tests[2].input = c;
    tests[2].expected_output= bubble_sort(c, 11);
    tests[2].size = 11;

    tests[3].input = d;
    tests[3].expected_output= bubble_sort(d, 7);
    tests[3].size = 7;

    tests[4].input = e;
    tests[4].expected_output= bubble_sort(e, 1);
    tests[4].size = 1;
}


void clearTests()
{
    for(int i = 0; i < TEST_SIZE; i++)
    {
        free(tests[i].expected_output);
    }
}


void printArr(int * arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%i, ", arr[i]);
    }
}



/**
 * Controlla la funzione sort esegua le operazioni correttamente
 * @param sorter puntatore a funzione int <>(int *, int)
 * @param input array di input da dare in pasto a sorter
 * @param output risultato atteso dal sort dell'array output
 * @param size lunghezza di input e output
 * 
 * @return 1 se l'esito e' positivo, 0 altrimenti
 */
int check_arr(int (*sorter)(int*, int), int * in, int * output, int size)
{
    //crea una copia degli array cosi che le funzioni possano modificarli

    int * input = malloc(size * sizeof(int));

    //non controllare se e' allocato correttamente, se manca la ram e' meglio che il programma crasha

    memcpy(input, in, size * sizeof(int));


    printf("\n > Input: "); printArr(input, size);

    //esegui il sort
    sorter(input, size);

    printf("\n > Got: "); printArr(input, size);
    printf("\n > Expected: "); printArr(output, size);

    //controlla che il risultato combabi con quello desiderato
    for(int i = 0; i < size; i++)
    {
        if(input[i] != output[i])
        {
            free(input);
            return 0;
        }
    }

    free(input);
    return 1;
}

int main(int argc, char *argv[])
{

    createTests();

    for(int i = 0; i < TEST_SIZE; i++)
    {
        printf("\nRunning test %i:", i);
        if(check_arr(merge_sort, tests[i].input, tests[i].expected_output , tests[i].size))
            printf("\nResult: OK\n",i);
        else
            printf("\nResult: ERROR\n",i);

        printf("\n");
    }

    clearTests();

    return 0;
}