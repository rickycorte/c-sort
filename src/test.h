#ifndef __C_TEST__
#define __C_TEST__

/**
 * Struttura dati di un test
 */
typedef struct 
{
    int * input;
    int * expected_output;
    int size;
} testCase;


/**
 * Carica da un file tutti gli array di numer di usare come test
 * 
 * @param tile_name nome del file da cui caricare i test
 * @param size variabile dove salvare la lunghezza dei test caricati
 * 
 * @return puntatore all'array dei test caricati, NULL se vi sono errori di caricamento
 */ 
testCase *loadTests(const char file_name[], int *size);


/**
 * Elimina tutte le allocazioni dei test
 * 
 * @param tests arry di test da eliminare
 * @param size lunghezza dell'array dei test da elimare
 */ 
void clearTests(testCase * tests, int size);


/**
 * Controlla la funzione sort esegua le operazioni correttamente
 * @param sorter puntatore a funzione int <>(int *, int)
 * @param in array di input da dare in pasto a sorter
 * @param output risultato atteso dal sort dell'array output
 * @param size lunghezza di input e output
 * 
 * @return 1 se l'esito e' positivo, 0 altrimenti
 */
int check_arr(int (*sorter)(int*, int), int * in, int * output, int size);


/**
 * Esegue il benchmark di una funzione
 * 
 * @param sorter puntatore a funzione int <>(int *, int)
 * @param in array di input da dare in pasto a sorter
 * @param output risultato atteso dal sort dell'array output
 * @param size lunghezza di input e output
 * @param exec_times numero di volte in cui va eseguito il test
 */
void benckmark(int (*sorter)(int*, int), int * in, int * output, int size, int exec_times);

#endif