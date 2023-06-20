#include <stdlib.h>

#define MAX_PQUEUE 50
#define MAX_COST 1000

#define PARENT(i) (i-1)/2
#define LEFT(i) 2*i + 1
#define RIGHT(i) 2*i + 2

#define BLACK 0
#define WHITE 1
#define GREY  2

typedef struct coord {
    int x;
    int y;
    int cost;
} *Coord;

typedef struct pQueue {
    Coord *coords;
    int pQ_size;
} PriorityQueue;

void c_swap(Coord h[], int a, int b) {
    Coord aux = h[a];
    h[a] = h[b];
    h[b] = aux;
}

int c_isEmpty (PriorityQueue *q) {
    return (q->pQ_size == 0);
}

void c_bubbleUp (int i, Coord h[]) {
    while (i > 0 && h[i]->cost < h[PARENT(i)]->cost) {
        c_swap(h, i, PARENT(i));
        i = PARENT(i);
    }
}

void c_bubbleDown (int i, Coord h[], int N) {
    int m;
    while (LEFT(i) < N) {
        m = (RIGHT(i) < N && h[LEFT(i)]->cost > h[RIGHT(i)]->cost) ? RIGHT(i) : LEFT(i);

        if (h[m]->cost > h[i]->cost)
            break;

        c_swap(h, i, m);
        i = m;
    }
}

void c_empty (PriorityQueue *q) {
    q->coords = malloc(sizeof(struct coord) * MAX_PQUEUE);
    q->pQ_size = 0;
}

void c_insert_pQueue (PriorityQueue *q, int x, int y, int cost) {
    if (q->pQ_size == MAX_PQUEUE) return;

    Coord new = malloc(sizeof(struct coord));
    new->x = x;
    new->y = y;
    new->cost = cost;

    q->coords[q->pQ_size] = new;
    c_bubbleUp(q->pQ_size, q->coords);
    q->pQ_size++;
}

void c_remove_pQueue (PriorityQueue *q) {
    if (q->pQ_size == 0) return;
    
    if (q->pQ_size == 1) {
        q->pQ_size = 0;
        return;
    }

    q->coords[0] = q->coords[q->pQ_size-1];
    q->pQ_size--;

    c_bubbleDown(0, q->coords, q->pQ_size);
}

Coord new_coord (int x, int y, int cost) {
    Coord new = malloc(sizeof(struct coord));
    new->x = x;
    new->y = y;
    new->cost = cost;
    return new;
}

Coord c_get_coord_with_lowest_cost (PriorityQueue *q) {
    if (q->pQ_size == 0) return NULL;

    Coord result = new_coord(q->coords[0]->x, q->coords[0]->y, q->coords[0]->cost);
    c_remove_pQueue(q);
    return result;
}


void procedure (int L, Coord coord, PriorityQueue *q, int vertex, int var_1, int var_2, char *mapa[], int color[], int cost[]) {  
    int aux;
    int new_cost = cost[vertex] + 1;

    if (var_1 == 0) aux = vertex + var_2;
    else aux = vertex + (var_1*L);

    if (mapa[coord->x+var_1][coord->y+var_2] == ' ' && (color[aux] == WHITE || (color[aux] == GREY && cost[aux] > new_cost))) {

        cost[aux] = new_cost;
        if (color[aux] == WHITE) {
            color[aux] = GREY;
            c_insert_pQueue(q, coord->x+var_1, coord->y+var_2, new_cost);
        } else {
            c_remove_pQueue(q);
            c_insert_pQueue(q, coord->x+var_1, coord->y+var_2, new_cost);
        }
    }

}

int caminho (int L, int C, char *mapa[L], int ls, int cs, int lf, int cf) {
    
    int start = cs + ls * L;
    int end = cf + lf * L;
    int size = L*C - 1;

    int color[size];
    int cost[size];

    for (int i = 0; i <= size; i++) {
        cost[i] = MAX_COST;
        color[i] = WHITE;
    }

    cost[start] = 0;

    Coord coord;
    PriorityQueue *q = malloc(sizeof(struct pQueue));
    c_empty(q);

    c_insert_pQueue(q, ls, cs, 0);
    int vertex, new_cost;

    while (!c_isEmpty(q)) {

        coord = c_get_coord_with_lowest_cost(q);
        vertex = coord->x * L + coord->y;

        color[vertex] = BLACK;

        if (vertex+1 <= size && coord->y+1 < C) procedure(L, coord, q, vertex,  0,  1, mapa, color, cost);
        if (vertex-1 >= 0 && coord->y-1 >= 0)   procedure(L, coord, q, vertex,  0, -1, mapa, color, cost);
        if (vertex-L >= 0)                      procedure(L, coord, q, vertex, -1,  0, mapa, color, cost);
        if (vertex+L <= size)                   procedure(L, coord, q, vertex,  1,  0, mapa, color, cost);

    }

    int result = cost[end];
    if (result >= 1000) result = -1;
    return result;
}