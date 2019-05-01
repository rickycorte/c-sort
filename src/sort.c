#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printState(int * arr, int from, int to)
{
    printf("%i-%i:", from, to);
    for(int i = from; i < to; i++)
    {
        printf(" %i", arr[i]);
    }
}


int merge_sort(int * arr, int size)
{
    if(!arr || size < 1) return 1;
    if(size == 1) return 0;

    int * swap_block = malloc(sizeof(int) * size/2);
    if(!swap_block) return 1; // impossibile allocare la ram necessaria

    //printf("\nArray size: %i, required steps: %i\n", size, size/2);

    int step = 1;

    // step esterni, emula la ricorsivita facendo size/2 cicli in cui si considerano
    // sotto-array con dimensione 2^(i+1)
    for(int i = 0; i < size/2; i++)
    {

        step = step * 2; // potenza di due in base al ciclio

        //printf("\nStart of Step %i, step size %i ", i, step);
        
        // itera tutti i sotto array per lo step considerato
        // eg: tutti gli array di 2, di 4, ecc ecc
        for(int sj = 0; sj < size; sj += step)
        {
            // controlla che non si vada in out of range
            int items = (sj + step > size)? size - sj : step;
            int swap_size = step / 2;
            if(items <= swap_size) continue;
            

            //indica dove comincia il secondo blocco da cui prendere gli elementi
            int arr_tail = sj + swap_size;
            

            //copia il primo blocco di elementi, da sj a sj + step/2
            memcpy(swap_block, arr + sj, swap_size * sizeof(int));

            //printf("\n   >> Swap ");
            //printState(swap_block, 0, swap_size);
            //printf("\n   >> Arr ");
            //printState(arr, arr_tail, sj + items);  

            // h indica la posizione che si ha su swap_block
            // j indica la posizione sull'array del secondo blocco
            int h = 0, j = 0;

            
            for(int itr = 0; itr < items; itr++)
            {

                if(h < swap_size && ( j >= items - swap_size || swap_block[h] < arr[arr_tail+ j]) )
                {
                    arr[sj + itr] = swap_block[h];
                    h++;
                }
                else
                {
                    arr[sj + itr] = arr[ arr_tail + j];
                    j++;
                }            
            }

            //printf("\n > Step %i, subarray %i ->  ", i, sj);
            //printState(arr, sj, sj + items);         
        }

        //printf("\nEnd of Step %i -> ", i);
        //printState(arr, 0, size);
        //printf("\n");
    }

    free(swap_block);
    return 0;
}


int * bubble_sort(int *arr, int size)
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