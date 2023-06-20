#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define NV  10
#define NE -1
#define INF 1000

#define WHITE 0
#define RED   1
#define BLUE -1

typedef struct aresta {
    int dest;
    int custo;
    struct aresta *prox;
} *LAdj, *GrafoL [NV];

typedef int GrafoM [NV][NV];

void fromMat (GrafoM in, GrafoL out) {
    for (int i = 0; i < NV; i++) {
        out[i] = NULL;
        for (int j = 0; j < NV; j++)
            if (in[i][j] != 0) {
                LAdj aux = malloc(sizeof(struct aresta));
                aux->dest = j;
                aux->custo = in[i][j];
                aux->prox = out[i];
                out[i] = aux;
            }
    }
}

void swap (int orla[], int a, int b) {
    int temp = orla[a];
    orla[a] = orla[b];
    orla[b] = temp;
}

int directed_to_undirected (GrafoM grafo) {
    for (int i = 0; i < NV; i++)
        for (int j = 0; j < NV; j++)
            if (grafo[i][j] == 0 && grafo[j][i] != 0) grafo[i][j] = grafo[j][i];
            else if (grafo[i][j] != 0 && grafo[j][i] == 0) grafo[j][i] = grafo[j][i];
}

void print_adjacency_list (GrafoL g) {
    LAdj list;
    for (int i = 0; i < NV; i++) {
        printf("Vertex %d\n", i);
        list = g[i];
        while (list != NULL) {
            printf("[%d ; %d] ", list->dest, list->custo);
            list = list->prox;
        }
        printf("\n");
    }
}


// maior componente ligada num grafo não orientado
int maxComp (GrafoL g) {
    
    int visited[NV] = {0};
    int color[NV] = {0};

    int cc_size;
    int max_size = 0;

    int queue[NV];
    int front, rear;

    for (int i = 0; i < NV; i++) {

        if (visited[i]) continue;
        
        front = 0;
        rear = 0; 
        cc_size = 1;

        queue[rear++] = i;
        visited[i] = 1;

        while (front < rear) {
            int curr = queue[front++];

            for (LAdj node = g[curr]; node != NULL; node = node->prox) {
                if (!visited[node->dest]) {
                    queue[rear++] = node->dest;
                    visited[node->dest] = 1;
                    cc_size++;
                }
            }

        }

        if (cc_size > max_size)
            max_size = cc_size;

    }

    return max_size;
}

int sccRec (GrafoL g, int *tStamp, int v, int color[], int t[], int M[]) {
    
    int r = 0;
    color[v] = BLUE;
    M[v] = t[v] = *tStamp++;

    for (LAdj node = g[v]; node != NULL; node = node->prox)
        if (color[node->dest] == WHITE) {
            r += sccRec(g, tStamp, node->dest, color, t, M);
            if (M[node->dest] < M[v]) M[v] = M[node->dest];
        } else if (color[node->dest] == BLUE) {
            if (M[node->dest] < M[v]) M[v] = M[node->dest];
        }

    if (M[v] == t[v]) r++;
    color[v] = RED;
    return r;
}

int tarjanSCC (GrafoL g) {

    int color[NV] = {WHITE};

    int time_stamps[NV], minimum_time_stamps[NV];
    for (int i = 0; i < NV; i++) {
        time_stamps[i] = -1;
        minimum_time_stamps[i] = -1;
    }

    int i, r = 0;
    int timeStamp = 0;

    for (int i = 0; i < NV; i++) 
        if (color[i] == WHITE)
            r += sccRec(g, &timeStamp, i, color, time_stamps, minimum_time_stamps);

    return r;

}

int main (void) {

    GrafoL gl, gl2;
    GrafoM gm = {{0, 5, 0, 2, 0, 0, 0, 0, 0, 0} // 0
                ,{0, 0, 0, 0, 8, 0, 0, 0, 0, 0} // 1
                ,{0, 0, 0, 0, 0, 2, 0, 0, 0, 2} // 2
                ,{0, 0, 0, 0, 0, 0, 2, 0, 0, 0} // 3
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 4
                ,{0, 0, 0, 0, 0, 0, 0, 0, 8, 0} // 5
                ,{0, 0, 0, 0, 0, 0, 0, 1, 0, 0} // 6
                ,{0, 0, 0, 2, 7, 0, 0, 0, 0, 0} // 7
                ,{0, 0, 0, 0, 3, 0, 0, 0, 0, 4} // 8
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 9
                };

    fromMat(gm, gl2);
    directed_to_undirected(gm);
    fromMat(gm, gl);
    print_adjacency_list(gl);

    int r = maxComp(gl);
    printf("\ncc_size: %d\n", r);

    int r_2 = tarjanSCC(gl2);
    printf("Tarjan: %d\n", r_2);
    
    return 0;
}