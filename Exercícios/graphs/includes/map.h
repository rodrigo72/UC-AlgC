#define NV_2 100

typedef struct vertice {
    int dest; int cost;
    struct vertice *prox;
} *Vertice, *Grafo [NV_2];

void map_to_graph(char *mapa[], Grafo grafo, int comprimento, int altura);
void print_graph (Grafo g);
void dijkskra_shortest_path(Grafo g, int k);
