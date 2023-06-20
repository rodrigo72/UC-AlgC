// GREEDY APPROACH (não é garantido que funcione em todos os casos)

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

void Display_solution (int *color) {

     printf("\nThe solution is the following :\n");
    for (int i = 0; i < NV; i++)
        printf("%d -> %d\n", i, color[i]);

}

// O(NV * E * c)
int Graph_Coloring (GrafoL g, int *color, int m) {
   
    // inicializa o primeiro nodo com uma cor
    color[0] = 1;
    for (int i = 1; i < NV; i++) color[i] = -1;
    
    // iterar por todos os nodos
    for (int i = 1; i < NV; i++) {
        // iterar por todas as cores
        for (int j = 1 ; j <= m ; j++) {
            // iterar por todos os nodos adjacentes ao nodo i
            for(LAdj node = g[i]; node != NULL; node = node->prox) {
                printf("v: %d, adj: %d, cor: %d\n", i, node->dest, j);
                color[i] = j;
                // se o nodo adjacente tiver a mesma cor, sair do ciclo e tentar outra cor
                if (color[node->dest] == j) {
                    color[i] = -1;
                    break;
                }
            }
            
            // se a cor é valida para todos os nodos adjacentes então tentar com o nodo seguinte
            if(color[i] != -1)
                break;
        }

        // a coloração não resultou // não são cores suficientes para o nodo e os seus nodos adjacentes
        if (color[i] == -1) return 0;
    }

    Display_solution(color);
    return 1;
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

    GrafoM gm2 = {{0, 5, 0, 0, 0, 0, 0, 0, 0, 0} // 0
                ,{0, 0, 0, 1, 0, 0, 0, 0, 0, 0} // 1
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 2
                ,{0, 0, 1, 0, 0, 0, 0, 0, 0, 0} // 3
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 4
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 5
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 6
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 7
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 8
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 9
                };

    directed_to_undirected(gm);
    fromMat(gm, gl);

    int color[NV];
    for (int i = 0; i < NV; i++) color[i] = -1;

    int k = 3;
    int r = Graph_Coloring(gl, color, k);
    if (r) printf("\nThe graph can be colored with %d <= k <= %d colors.\n", k, NV);
    else printf("\nThe graph cannot be colored with %d colors.\n", k);

    return 0;
}