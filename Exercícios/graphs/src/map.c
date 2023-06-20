#include "map.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_PQUEUE 50
#define MAX_COST 1000

#define PARENT(i) (i-1)/2
#define LEFT(i) 2*i + 1
#define RIGHT(i) 2*i + 2

typedef struct pQueuePair {
    int index;
    int cost;
} *Pair;

typedef struct pQueue {
    Pair *pairs;
    int pQ_size;
} PriorityQueue;

void swap (Pair h[], int a, int b) {
    Pair aux = h[a];
    h[a] = h[b];
    h[b] = aux;
}

int isEmpty (PriorityQueue *q) {
    return (q->pQ_size == 0);
}

void bubbleUp (int i, Pair h[]) {
    while (i > 0 && h[i]->cost < h[PARENT(i)]->cost) {
        swap(h, i, PARENT(i));
        i = PARENT(i);
    }
}

void bubbleDown (int i, Pair h[], int N) {
    int m;
    while (LEFT(i) < N) {
        m = (RIGHT(i) < N && h[LEFT(i)]->cost > h[RIGHT(i)]->cost) ? RIGHT(i) : LEFT(i);

        if (h[m]->cost > h[i]->cost)
            break;

        swap(h, i, m);
        i = m;
    }
}

void empty (PriorityQueue *q) {
    q->pairs = malloc(sizeof(struct pQueuePair) * MAX_PQUEUE);
    q->pQ_size = 0;
}

void insert_pQueue (PriorityQueue *q, int index, int cost) {
    if (q->pQ_size == MAX_PQUEUE) return;

    Pair new = malloc(sizeof(Pair));
    new->index = index;
    new->cost = cost;

    q->pairs[q->pQ_size] = new;
    bubbleUp(q->pQ_size, q->pairs);
    q->pQ_size++;
}

void remove_pQueue (PriorityQueue *q) {
    if (q->pQ_size == 0) return;
    
    if (q->pQ_size == 1) {
        q->pQ_size = 0;
        return;
    }

    q->pairs[0] = q->pairs[q->pQ_size-1];
    q->pQ_size--;

    bubbleDown(0, q->pairs, q->pQ_size);
}

int get_index_with_lowest_cost (PriorityQueue *q) {
    if (q->pQ_size == 0) return -1;
    int result = q->pairs[0]->index;
    remove_pQueue(q);
    return result;
}

void print_graph (Grafo g) {
    Vertice list;
    int check;
    for (int i = 0; i < NV_2; i++) {
        list = g[i];
        check = (list != NULL);
        if (check) printf("Vertex %d\n", i);

        while (list != NULL) {
            printf("[%d] ", list->dest);
            list = list->prox;
        }
        if (check) printf("\n");
    }
}

Vertice new_vertice(int dest, int cost, Vertice prox) {
    Vertice new = malloc(sizeof(struct vertice));
    new->cost = cost;
    new->dest = dest;
    new->prox = prox;
    return new;
}

void map_to_graph (char *mapa[], Grafo grafo, int comprimento, int altura) {
    Vertice curr;
    int vertice = 0;
    for (int i = 0; i < NV_2; i++) grafo[i] = NULL;

    for (int i = 0; i < altura; i++)
        for (int j = 0; j < comprimento; j++) {
            curr = NULL;
            if (mapa[i][j] != '#') {
                if (j+1 < comprimento && mapa[i][j+1] != '#') {
                    Vertice new = new_vertice(vertice+1, 1, curr);
                    curr = new;
                }

                if (j-1 >= 0 && mapa[i][j-1] != '#') {
                    Vertice new = new_vertice(vertice-1, 1, curr);
                    curr = new;                  
                }
                if (i+1 < altura && mapa[i+1][j] != '#') {
                    Vertice new = new_vertice(vertice+comprimento, 1, curr);
                    curr = new;                    
                }

                if (i-1 >= 0 && mapa[i-1][j] != '#') {
                    Vertice new = new_vertice(vertice-comprimento, 1, curr);
                    curr = new;                    
                }

                grafo[vertice] = curr;
            }
            vertice++;
        }
}

#define BLACK 0
#define WHITE 1
#define GREY  2

void dijkskra_shortest_path(Grafo g, int k) {

    int color[NV_2];
    int cost[NV_2];
    int prev[NV_2];

    for (int i = 0; i < NV_2; i++) {
        cost[i] = MAX_COST;
        color[i] = WHITE;
    }

    cost[k] = 0;
    prev[k] = -1;
    color[k] = GREY;

    PriorityQueue *q = malloc(sizeof(struct pQueue));
    empty(q);
    insert_pQueue(q, k, 0);

    int new_cost, color_dest;
    int current_vertex;

    while (!isEmpty(q)) {

        current_vertex = get_index_with_lowest_cost(q);
        color[current_vertex] = BLACK;

        for (Vertice aux = g[current_vertex]; aux != NULL; aux = aux->prox) {
            new_cost = cost[current_vertex] + aux->cost;
            color_dest = color[aux->dest];

            if (color_dest == WHITE || (color_dest == GREY && cost[aux->dest] > new_cost)) {
                cost[aux->dest] = new_cost;
                prev[aux->dest] = current_vertex;
    
                if (color_dest == WHITE) {
                    color[aux->dest] = GREY;
                    insert_pQueue(q, aux->dest, new_cost);
                } else { // acho que não é preciso atualizar
                    // remove_pQueue(q);
                    // insert_pQueue(q, aux->dest, new_cost);
                }
            }
        }
    }

    printf("%d\n", cost[18]);

}