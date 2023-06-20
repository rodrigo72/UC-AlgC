#include "min_heap_ficha3.h"

#include <stdio.h>
#include <stdlib.h>

// -------------------------------------------------------------------------------------------
// A noção de min-heap utilizada neste módulo difere de uma árvore binária de procura;       |
// (!) É uma árvore binária em que cada nodo é menor ou igual a todos os seus sucessores (!) |
// -------------------------------------------------------------------------------------------

void swap(int h[], int a, int b) {
    int aux = h[a];
    h[a] = h[b];
    h[b] = aux;
}

// Pior caso: log(N)
// Número de comparações e trocas: 2 * log(N)
// Troca enquanto houver um valor maior acima
void bubbleUp (int i, int h[]) {
    while (i > 0 && h[i] < h[PARENT(i)]) {
        swap(h, i, PARENT(i));
        i = PARENT(i);
    }
}

// Pior caso : log(N)
// Troca enquanto houver um valor mais pequeno em baixo
void bubbleDown (int i, int h[], int N) {
    int m;
    // Enquanto a posição do nodo da esquerda seja menor que N, ou seja, caso exista pelo menos um primeiro nodo no nível seguinte
    while (LEFT(i) < N) {
        // se o nodo à direita não existe ent m = posiçao do nodo da esquerda
        // caso contrario, se o valor do nodo à esquerda for maior que o valor do nodo à direita então m = posição do nodo da direita
        // caso contrário m = posição do nodo da esquerda
        m = (RIGHT(i) < N && h[LEFT(i)] > h[RIGHT(i)]) ? RIGHT(i) : LEFT(i);

        // break se o valor da posição m é maior do que o valor da posição i
        if (h[m] > h[i])
            break;

        // se o valor da posição m for menor ou igual ao valor da posição i
        swap(h, i, m);
        i = m;
    }
}

void empty (PriorityQueue *q, int N) {
    q->valores = (int *) malloc(sizeof(int) * N);
    q->max = N;
    q->tamanho = 0;
}

int isEmpty (PriorityQueue *q) {
    return (q->tamanho == 0);
}

int add (int x, PriorityQueue *q) {
    if (q->tamanho == q->max) return 1;

    q->valores[q->tamanho] = x;
    bubbleUp(q->tamanho, q->valores);
    q->tamanho++;

    return 0;
}

int remove_q (PriorityQueue *q, int *rem) {
    if (q->tamanho == 0) return 1;
    
    *rem = q->valores[0]; 
    if (q->tamanho == 1) {
        q->tamanho = 0;
        return 0;
    }
    // como se diminui o tamanho ao retirar o menor elemento (o primeiro), 
    // coloca-se o ultimo elemento no lugar do menor,
    // e ordena-se esse elemento com base nas regras da min-heap usada
    // (cada nodo é menor ou igual a todos os seus sucessores)
    q->valores[0] = q->valores[q->tamanho-1];
    q->tamanho--;

    bubbleDown(0, q->valores, q->tamanho);
    return 0;
}

void heapify_1 (int v[], int N) {
    for (int i = 1; i < N; i++) {
        bubbleUp(i, v);
    }
}

void heapify_2 (int v[], int N) {
    for (int i = PARENT(N-1); i >= 0; i--) {
        bubbleDown(i, v, N);
    }
}

void ordenaHeap (int h[], int N) {
    for (int i = 1; i < N; i++) {
        swap(h, 0, N-i);
        bubbleDown(i, h, N-i);
    }
}

void printHeap (int h[], int N) {
    printf("\n{ ");
    for (int i = 0; i < N; i++) {
        printf("%d ", h[i]);
    }
    printf("}\n");
}