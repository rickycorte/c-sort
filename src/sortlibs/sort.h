#ifndef C__SORT
#define C__SORT

/**
 * Merge sort (iterative)
 * 
 * @param arr array to sort
 * @param size arr size
 * 
 * @return 0 on success, 1 on error
 * */
int merge_sort(int * arr, int size);


/**
 * Insertion sort (iterative)
 * 
 * @param arr array to sort
 * @param size arr size
 * 
 * @return 0 on success, 1 on error
 * */
int insertion_sort(int *arr, int size);

/**
 * Bubble sort (iterative)
 * 
 * @param arr array to sort
 * @param size arr size
 * 
 * @return 0 on success, 1 on error
 * */
int bubble_sort(int *arr, int size);


/**
 * Heap sort (recursive)
 * 
 * @param arr array to sort
 * @param size arr size
 * 
 * @return 0 on success, 1 on error
 * */
int heap_sort(int *arr, int size);


#endif