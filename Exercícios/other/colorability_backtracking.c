// BACKTRACKING APPROACH

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NV 10

typedef struct aresta {
    int dest;
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
                aux->prox = out[i];
                out[i] = aux;
            }
    }
}

int directed_to_undirected (GrafoM grafo) {

    for (int i = 0; i < NV; i++)
        for (int j = 0; j < NV; j++)
            if (grafo[i][j] == 0 && grafo[j][i] != 0) grafo[i][j] = grafo[j][i];
            else if (grafo[i][j] != 0 && grafo[j][i] == 0) grafo[j][i] = grafo[j][i];

}

void display_solution (int *color) {

    printf("\nThe solution is the following :\n");
    for (int i = 0; i < NV; i++)
        printf("%d -> %d\n", i, color[i]);

}

int check_availability (GrafoL g, int v, int *color, int c){

    for (LAdj node = g[v]; node != NULL; node = node->prox)
        if (color[node->dest] == c)
            return 0;

    return 1;
}

int graph_coloring_trace (GrafoL g, int m, int *color, int v) {

    if (v == NV)
        return 1;

    for (int c = 1; c <= m; c++) {
        if (check_availability(g, v, color, c)) {
            color[v] = c;
            if (graph_coloring_trace(g, m, color, v+1))
                return 1;

            color[v] = 0;
        }
    }
    
    return 0;
}

int graph_coloring (GrafoL g, int *color,int m) {

    for (int i = 0; i < NV; i++) color[i] = -1;

    if (graph_coloring_trace(g, m, color, 0)){
        display_solution(color);
        return 1;
    }

    return 0;
}

int main(void) {

    GrafoL gl;
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

    directed_to_undirected(gm);
    fromMat(gm, gl);

    int color[NV];
    for (int i = 0; i < NV; i++) color[i] = -1;

    int k = 3;
    int r = graph_coloring(gl, color, k);
    if (r) printf("\nThe graph can be colored with %d <= k <= %d colors.\n", k, NV);
    else printf("\nThe graph cannot be colored with %d colors.\n", k);

    return 0;
}