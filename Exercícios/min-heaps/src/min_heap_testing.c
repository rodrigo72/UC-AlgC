#include "min_heap_testing.h"
#include "min_heap_ficha3.h"
#include <stdlib.h>
#include <stdio.h>

int max(int a, int b) {
    return (a > b ) ? a : b;
}

// Complexidade: N * log(k) + (N-k) * log(k) = O(N*log(k))
void k_largest_elements(int v[], int N, int k) {

    PriorityQueue *q = malloc(sizeof(struct pQueue));
    empty(q, k);

    int i = 0;
    for (;i < k; i++) {
        add(v[i], q);
    }

    int m, aux;
    for (;i < N; i++) {
        if (v[i] > q->valores[0]) {
            remove_q(q, &aux);
            add(v[i], q);
        }
    }

    printf("{ ");
    for (int c = 0; c < k; c++) {
        printf("%d ", q->valores[c]);
    }
    printf("}\n");
}