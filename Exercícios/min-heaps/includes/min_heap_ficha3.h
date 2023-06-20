#ifndef MIN_HEAP_FICHA3_H_
#define MIN_HEAP_FICHA3_H_

#define PARENT(i) (i-1)/2  // os indices do array começam em 0 
#define LEFT(i) 2*i + 1
#define RIGHT(i) 2*i + 2

typedef int Elem;  // elementos da heap.

typedef struct pQueue {
    int *valores;
    int max;
    int tamanho;
} PriorityQueue;

typedef struct {
    int   size;
    int   used;
    Elem  *values;
} Heap;

void bubbleUp (int i, int h[]);
void bubbleDown (int i, int h[], int N);
void empty (PriorityQueue *q, int N);
int isEmpty (PriorityQueue *q);
int add (int x, PriorityQueue *q);
int remove_q (PriorityQueue *q, int *rem);
void heapify_1 (int v[], int N);
void heapify_2 (int v[], int N);
void ordenaHeap (int h[], int N);
void printHeap (int h[], int N);

#endif // MIN_HEAP_FICHA3_H_