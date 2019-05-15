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
    if(!swap_block) return 1; // can't allocate ram

    int *source = arr, *dest = swap_block;
    int h = 0, k = 0, items = 0, base = 0;

    int ch, ck; // index cache

    // iterate subarrays sizes
    for(int step = 2; step/2 < size; step *= 2)
    {
        //printf("\nStep %i start:", step);
        for(int i = 0; i < size; i++)
        {
            // new subarray
            if(i % step == 0)
            {
                h = 0;
                k = 0;
                items = size - i;
                if(items > step) items = step;
                //printf("\n  ~~> Array start %i", i);

                // this sub array is too short
                // and have just been ordered in a previous step
                // so we just copy it
                if(items < step/2) 
                {
                    memcpy(dest + i, source + i, items * sizeof(int));
                    //printf(" -> skipped");
                    break;
                }

                base = i; // start index of the subarray
            }

            // merge the two "virtual" subarrays to emulate recurion
            // base is the base array index
            // h is the index of the fist unmerged element of subarray 1
            // k is the index of the fist unmerged element of subarray 2
            // ch, ck are cached index to reduce math operations
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

        // swap the array to use as destination of merge 
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

    // We need to make sure that the data is written to the right array
    // the one passed as function paramter
    // this check seem wrong because the last for iteration swaps the arrays
    // so we need to use the opposite condition to check if the data is on the right array
    if(dest == arr)
    {
        memcpy(arr, swap_block, size * sizeof(int));
    }
    

    free(swap_block);
    return 0;
}


int bubble_sort(int *arr, int size)
{
    if(!arr || size < 1) return 1;
    if(size == 1) return 0;

    int temp;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < i; j++)
        {
            if(arr[j] > arr[i])
            {
                temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            } 
        }
    }

    return 0;
}


int insertion_sort(int *arr, int size)
{
    if(!arr || size < 1) return 1;
    if(size == 1) return 0;

    int j, value;

    for(int i = 1; i < size; i++)
    {
        value = arr[i];
        j = i - 1;

        // move right all elements to make free the "insertion spot"
        while (j >= 0 && arr[j] > value)
        {
            arr[j+1] = arr[j]; 
            j--;
        }
        arr[j+1] = value;  
    }

    return 0;
}





static void max_heapify(int* arr, int size, int i)
{
    int l = 2*i + 1;
    int r = l+1;
    int max = i;

    //find max
    if(l < size && arr[l] > arr[max])
        max = l;
    
    if(r < size && arr[r] > arr[max])
        max = r;
    
    if(max != i)
    {
        int t = arr[i];
        arr[i] = arr[max];
        arr[max] = t;

        max_heapify(arr, size, max);
    }
}

int heap_sort(int *arr, int size)
{
    if(!arr) return 1;

    for(int i = size / 2 - 1; i >= 0; i--)
    {
        max_heapify(arr, size, i);
    }

    int t;

    for(int i = size-1; i > 0; i--)
    {
        t = arr[0];
        arr[0] = arr[i];
        arr[i] = t;
        max_heapify(arr, i, 0);
    }

    return 0;
}


static inline int quick_partition(int *arr, int start, int end)
{
    int x = arr[end];
    int i = start - 1;

    for(int j = start; j < end; j++)
    {
        if(arr[j] <= x)
        {
            i += 1;
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }

    int t = arr[i+1];
    arr[i+1] = arr[end];
    arr[end] = t;

    return i + 1;
}

static inline void quick_sort_internal(int *arr, int start, int end)
{
    if(start < end)
    {
        int q = quick_partition(arr, start, end);
        quick_sort_internal(arr, start, q-1);
        quick_sort_internal(arr, q+1, end);
    }
}

int quick_sort(int *arr, int sz)
{
    if(!arr) return 1;

    quick_sort_internal(arr, 0, sz-1);

    return 0;
}