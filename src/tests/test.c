#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/**
 * Bubble sort usato da load tests per oridinare in modo sicuro e lento gli array da testare
 * Perche bubble sort? perche ha un implementazione semplice e molto standard quindi e' difficile avere dei bug
 * Inoltre nella fase di caricamento non e' richiesta velocita ma sicurezza sui dati caricati per i test
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
 * Printa un array nel terminale 
 * 
 * @param arr array da printare
 * @param size grandezza dell'array
 */
static void printArr(int * arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%i, ", arr[i]);
    }
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
            if(*size >= test_allocated_size)
            {
                test_allocated_size += 5;
                tests = realloc(tests, test_allocated_size * sizeof(testCase));
            }    

            //creo il nuovo test
            tests[*size].input = arr;
            tests[*size].expected_output = bubble_sort(arr, arr_size);
            tests[*size].size = arr_size;

            (*size)++;

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
    //crea una copia degli array cosi che le funzioni possano modificarli

    int * input = malloc(size * sizeof(int));

    //non controllare se e' allocato correttamente, se manca la ram e' meglio che il programma crasha

    memcpy(input, in, size * sizeof(int));  

    //esegui il sort
    sorter(input, size);


    //controlla che il risultato combabi con quello desiderato
    for(int i = 0; i < size; i++)
    {
        if(input[i] != output[i])
        {
            free(input);
            //printa info solo su test falliti
            printf("\n > Input: "); printArr(in, size);
            printf("\n > Got: "); printArr(input, size);
            printf("\n > Expected: "); printArr(output, size);
            return 0;
        }
    }

    free(input);
    return 1;
}


void benckmark(int (*sorter)(int*, int), int * in, int * output, int size, int exec_times)
{
    clock_t start_tm, best_tm, worst_tm, sum_tm = 0;

    double avg;

    // esegui n volte il benckmark delle funzione con gli stessi dati
    for(int i = 0; i < exec_times; i++)
    {
        start_tm = clock();
        check_arr(sorter, in, output, size);
        start_tm = clock() - start_tm;

        sum_tm = start_tm;

        // ricordati il miglior e il peggior tempo di esecuzione
        if(i == 0)
        {
            best_tm = start_tm;
            worst_tm = start_tm;
        }
        else
        {
            if(start_tm < best_tm) best_tm = start_tm;
            if(best_tm > worst_tm ) worst_tm = start_tm;
        }
        
        sum_tm += start_tm;
    }
    
    avg = ((double)sum_tm) / exec_times;

    printf("BENCHMARK > Avg: %f Best: %ld Worst: %ld (Avg Time: %fs)", avg, best_tm, worst_tm, avg/CLOCKS_PER_SEC ); 

}