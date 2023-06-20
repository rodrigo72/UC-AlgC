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
                aux->custo = in[i][j] > 0 ? in[i][j] * (-1) : in[i][j];
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

int find_min_index (int orla[], int pesos[], int size) {
    int r = 0;
    for (int i = 0; i < size; i++) 
        if (pesos[orla[i]] < pesos[orla[r]]) r = i;
    
    return r;
}

int dijkstraSP (GrafoL g, int or) {
    int orla[NV];
    int cor[NV], pais[NV], pesos[NV];

    for (int i = 0; i < NV; i++) {
        pais[i] = -2, pesos[i] = INF, cor[i] = WHITE;
    }

    int size = 0;
    orla[size++] = or, cor[or] = BLUE, pesos[or] = 0, pais[or] = -1;

    int i, curr;
    while (size > 0) {
        i = find_min_index(orla, pesos, size);
        curr = orla[i];
        swap(orla, i, --size);
        cor[curr] = RED;

        for (LAdj node = g[curr]; node != NULL; node = node->prox) {
            if (cor[node->dest] == WHITE) {
                orla[size++] = node->dest;
                pais[node->dest] = curr;
                cor[node->dest] = BLUE;
                pesos[node->dest] = pesos[curr] + node->custo;
            } else if (cor[node->dest] == BLUE && pesos[node->dest] > pesos[curr] + node->custo) {
                pais[node->dest] = curr;
                pesos[node->dest] = pesos[curr] + node->custo;
            }
        }
    }

    int max = pesos[0];
    for (int i = 0; i < NV; i++) {
        if (pesos[i] < max && pesos[i] != INF) max = pesos[i];
    }
    
    return max;
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

    fromMat(gm, gl); // inverte os custos
    print_adjacency_list(gl);
    int r = dijkstraSP(gl, 2);
    printf("r: %d\n", r);

    // Não calcula com exatidão o caminho mais longo, mesmo invertendo os custos, visto que existem ciclos
    // Ciclos, no entanto, não seriam um problema ao encontrar o caminho mais curto num grafo com custos positivos
    // "Dijkstra's marking of visited nodes will not allow to find long paths", tal como se repara começando 
    // em 2; o 9 fica visitado e por isso com custo fixo de -2, enquanto que na verdade, o caminho mais
    // longo acaba em 9, e como já foi marcado como visitado, passa a acabar em 4

}