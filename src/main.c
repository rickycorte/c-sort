#include "sort.h"
#include <stdio.h>

int main(int argc, char *argv[])
{

    int arr[] = { 80, 43, 3 ,1 ,5, 763, -1}; 

    printf("Original:\n");
    for(int i = 0; i < 7; i++)
    {
        printf(" %i", arr[i]);
    }
    merge_sort(arr, 7);

    printf("\nSorted:\n ");
    for(int i = 0; i < 7; i++)
    {
        printf(" %i", arr[i]);
    }

    printf("\n");
}