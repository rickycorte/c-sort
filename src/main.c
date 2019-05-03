#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct 
{
    int * input;
    int * expected_output;
    int size;
} testCase;


testCase *tests;
int tests_size = 0;


int success = 0, fails = 0;


void printArr(int * arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%i, ", arr[i]);
    }
}


//TODO: controllare le riallocazioni
void createTests()
{
    FILE *fp;

    fp = fopen("test_data.txt", "r");
    if(!fp)
    {
        printf("Missing test_data.txt\n");
        return;
    }

    char c;
    char num_buffer[10];
    int used_buffer = 0;

    int *arr = NULL, arr_size = 0, arr_allocated_size = 0;
    int test_allocated_size = 0;

    memset(num_buffer, 0, 10); // imposta a 0 tutto il buffer per il numero

    while( (c = fgetc(fp)) != EOF)
    {


        //trovato numero
        if(c == ' ' || c == '\n')
        {
            int num = atoi(num_buffer);
            memset(num_buffer, 0, 10); // imposta a 0 tutto il buffer per il numero
            used_buffer = 0;

            //espandi l'array o allocalo se e' la prima esecuzione
            if(arr_size >= arr_allocated_size)
            {               
                arr_allocated_size += 5;
                arr = realloc(arr, arr_allocated_size * sizeof(int));             
            }

            //aggiungi il numero all'array
            arr[arr_size] = num;
            arr_size++;
        }

        //trovato fine array
        if(c == '\n')
        {
            if(arr_size < 1) 
            {
                printf("Format error, arrays must have at leat one element!\n");
                continue;
            }

            //espandi array dei test se necessario
            if(tests_size >= test_allocated_size)
            {
                test_allocated_size += 5;
                tests = realloc(tests, test_allocated_size * sizeof(testCase));
            }    

            //creo il nuovo test
            tests[tests_size].input = arr;
            tests[tests_size].expected_output = bubble_sort(arr, arr_size);
            tests[tests_size].size = arr_size;

            tests_size++;

            //reset dei valori per il prossimo array
            arr = NULL;
            arr_allocated_size = 0;
            arr_size = 0;
            continue;   
        }

        //aggiungi il carattere del numero
        if(c != ' ' && c != '\n')
        {
            num_buffer[used_buffer] = c;
            used_buffer++;
        }

        // esegui un range check impreciso del valore massimo
        if(used_buffer >= 9) 
        {
            printf("Format error! Cant accept numbers bigger than 2 * 10^9\n");
            return;
        }

    }


    fclose(fp);

}


void clearTests()
{
    for(int i = 0; i < tests_size; i++)
    {
        free(tests[i].input);
        free(tests[i].expected_output);
    }

    free(tests);
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

    for(int i = 0; i < tests_size; i++)
    {
        printf("\nRunning test %i:", i);
        if(check_arr(merge_sort, tests[i].input, tests[i].expected_output , tests[i].size))
        {
            printf("\nResult: OK\n",i);
            success++;
        }
        else
        {
            printf("\nResult: ERROR\n",i);
            fails++;
        }

        printf("\n");
    }

    printf("\nFINAL RESULT: %s\n", (success == tests_size)? "SUCCESS" : "FAIL");
    printf("In %i tests: %i successes and %i fails\n", tests_size, success, fails);

    clearTests();

    return 0;
}