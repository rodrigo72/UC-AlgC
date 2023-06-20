#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#define NV 10
#define MAX 10000
#define MIN -10000
#define MAX_2 20

typedef struct aresta {
    int dest; int custo;
    struct aresta *prox;
} *LAdj, *GrafoL [NV];

typedef int GrafoM [NV][NV];

void fromMat (GrafoM in, GrafoL out);
void fromMat_2 (GrafoM in, GrafoL out);
void print_adjacency_list (GrafoL g);
void inverte (GrafoL in, GrafoL out);
int adj (GrafoL g, int o, int d);
int in_degree(GrafoL g);
int colorOK (GrafoL g, int cor[]);
int homomorfOK (GrafoL g, GrafoL h, int f[]);
void topological_order (GrafoL g, int k);
int biPartite (GrafoL g);
int maisLonga (GrafoL g, int or, int p[]);
int maisLonga_2 (GrafoL g, int or, int p[]);
int componentes (GrafoL g, int c[]);
int directed_to_undirected (GrafoM grafo);

#endif // ADJACENCY_LIST_H