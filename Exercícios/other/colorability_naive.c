// NAIVE APPROACH (testar todas as combinações)

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

int check_Availability (GrafoL g, int *color){

    for (int i = 0; i < NV; i++)
        for (LAdj node = g[i]; node != NULL; node = node->prox)
            if (color[i] == color[node->dest])
                return 0;

    return 1;
}

void Display_solution (int *color) {

     printf("\nThe solution is the following :\n");
    for (int i = 0; i < NV; i++)
        printf("%d -> %d\n", i, color[i]);

}

// k : number of colors
// i : start
// color : color combination
// Complexidade : O(k^(NV*2))
int Graph_Coloring (GrafoL g, int k, int i, int *color) {
    
    // Quando o array color está completo
    // Verifica se existem nodos adjacentes com a mesma cor
    // se não existerem a combinação de cores é válida
    if (i == NV)
        if (check_Availability(g, color)) {
            Display_solution(color);
            return 1;
        } else 
            return 0;
    
    // todas a cobinações para o index i
    // e recursivamente para todos os outros
    for (int j = 1; j <= k; j++) {
        color[i] = j;
        if (Graph_Coloring(g, k, i+1, color)) return 1;
        color[i] = 0;
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

    int k = 10;
    int r = Graph_Coloring(gl, k, 3, color);
    if (r) printf("\nThe graph can be colored with %d <= k <= %d colors.\n", k, NV);
    else printf("\nThe graph cannot be colored with %d colors.\n", k);

    return 0;
}