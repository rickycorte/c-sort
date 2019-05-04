#ifndef C__SORT
#define C__SORT

/**
 * Merge sort array
 * @param arr array to sort
 * @param size arr size
 * 
 * @return 0 on success, 1 on error
 * */
int merge_sort(int * arr, int size);

/**
 * Implementazione piu facile per ordinare i valori dei test in modo sicuro
 * evitando eventuali bug del codice da testare
 * 
 * @param arr array da ordinare
 * @param size lunghezza di arr
 * @return una copia di arr riordinato
 */
int * bubble_sort(int *arr, int size);

#endif