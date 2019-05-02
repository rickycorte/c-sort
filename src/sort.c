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

    int * swap_block = malloc(sizeof(int) * size);
    if(!swap_block) return 1; // impossibile allocare la ram necessaria

    int *source = arr, *dest = swap_block;
    int h = 0, k = 0, items = 0, base = 0;

    int ch, ck; // index cache

    for(int step = 2; step/2 < size; step *= 2)
    {
        //printf("\nStep %i start:", step);
        for(int i = 0; i < size; i++)
        {
            //nuovo sottarray
            if(i % step == 0)
            {
                h = 0;
                k = 0;
                items = size - i;
                if(items > step) items = step;
                //printf("\n  ~~> Array start %i", i);

                //oridinati dagli stadi precedenti
                //copia soltanto sull'altro array e termina
                if(items < step/2) 
                {
                    memcpy(dest + i, source + i, items * sizeof(int));
                    //printf(" -> skipped");
                    break;
                }

                base = i; // partenza del sotto array
            }

            ch = base + h;
            ck = base + step/2 + k;
            if(h < step/2 && ( step/2 + k >= items || source[ch] < source[ck]) )
            {
                dest[i] = source[ch];
                //printf("\n  ~~~ Swap of %i: base %i, ch %i, ck %i -> %i", i, base, ch, ck, source[ch] );
                h++;
            }
            else
            {
                dest[i] = source[ck];
                //printf("\n  ~~~ Swap of %i: base %i, ch %i, ck %i -> %i", i, base, ch, ck, source[ck] );
                k++;

            }           
        }

        //printf("\nStep %i result:", step);
        //printf("\n  >>> source: ");
        //printState(source, 0, size);
        //printf("\n  >>> dest: ");
        //printState(dest, 0, size);

        //swappa alternativamente i due array su cui riodinare
        if(source == arr)
        {
            source = swap_block;
            dest = arr;
        }
        else
        {
            source = arr;
            dest = swap_block;
        }      
    }

    // che scelta infame del tipo della funzione :L
    // mi tocca assicurarmi che i valori siano sull'array giusto
    //ps: sono swappati prima del termine del ciclio quindi fa bordello
    if(dest == arr)
    {
        memcpy(arr, swap_block, size * sizeof(int));
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