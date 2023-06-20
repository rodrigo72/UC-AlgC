#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define USED 1
#define FREE 0

typedef struct node {
    int status;
    int key;
} Node;

#define NV 10

typedef struct aresta {
    int dest, peso;
    struct aresta *prox;
} *Grafo[NV];

int ht_add (Node v[], int x, int N) {

    int count;
    int pos = x % N;

    for (count = 0; count < N && v[pos].status != FREE; count++) {
        if (v[pos].key == x) return 1;
        else pos = (pos + 1) % N;
    }

    if (count == N) return -1;

    v[pos].status = USED;
    v[pos].key = x;

    return 0;

}

int repetidos (int v[], int N) {

    Node ht[N];
    memset(ht, 0, sizeof(struct node)*N);

    for (int i = 0; i < N; i++) {
        if (ht_add(ht, v[i], N))
            return 1;
    }

    return 0;
}

#define WHITE 0
#define RED   1
#define BLUE -1

int isTree (Grafo g) {
    
    int index = 0, found = 0;
    int indegree[NV] = {0};

    for (int i = 0; i < NV; i++)
        for (struct aresta *node = g[i]; node != NULL; node = node->prox)
            indegree[node->dest]++;
        
    for (int i = 0; i < NV; i++)
        if (indegree[i] == 0) {
            index = i;
            found++;
        }

    if (found != 1) return -1;

    int queue[NV], cor[NV] = {0};
    int front = 0, rear = 0;

    queue[rear++] = index;

    while (front < rear) {
        int curr = queue[front--];
        cor[curr] = RED;

        for (struct aresta *node = g[curr]; node != NULL; node = node->prox) {
            if (cor[node->dest] != WHITE) return -1;
            else {
                cor[node->dest] = BLUE;
                queue[rear++] = node->dest;
            }
        }
    }    

    return index;
}

int main (void) {

    int x = 17;
    int arr[] = {0, 1, 9, 4, 5, 12, 6, 9087, 3, 23, 333, 42, 55, 66, 987, 32, 46};
    printf("%d\n", repetidos(arr, x));

    return 0;
}