#include "min_heap.h"
#include "min_heap_ficha3.h"
#include "min_heap_testing.h"

#include <stdlib.h>
#include <time.h>

int main(void) {

    AVLTree *a = malloc(sizeof(struct avl));
    updateAVL(a, 1, 1);
    updateAVL(a, 2, 2);
    updateAVL(a, 3, 3);
    updateAVL(a, 4, 4);
    updateAVL(a, 5, 5);
    updateAVL(a, 6, 6);
    updateAVL(a, 7, 7);
    print_t(*a);

    int k = 2;
    int N = 100;
    int max = 1000;

    int arr[N-1];
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        arr[i] = rand() % max;
    }

    k_largest_elements(arr, N, k);
    printHeap(arr, N);

    return 0;
}