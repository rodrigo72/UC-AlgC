#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define NV 10

typedef struct aresta {
    int dest;
    int custo;
    struct aresta *prox;
} *LAdj, *GrafoL [NV];

typedef int GrafoM [NV][NV];

void swap (int v[], int i, int j){
    int tmp = v[i];
    v[i] = v[j]; v[j] = tmp;
}

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

int minIndPeso (int v[], int pesos[], int N){
     int i, r = 0;
     for (i = 1; i < N; i++)
         if (pesos[v[i]] < pesos[v[r]]) r = i;
     return r;
 }

int dijkstraSP (GrafoL g, int or, int pais[], int pesos[]) {
    int r, i, v, cor [NV], 
        orla[NV], tam; 
    LAdj x;
    // inicializacoes
    for (i=0; i<NV; i++){
         pais[i] = -2; cor[i] = 0 ; // nao visitado
    }
    r = 0; orla[0] = or; tam = 1; 
    pesos[or] = 0; pais[or] = -1; cor [or] = 1; // na orla
    // ciclo 
    while (tam>0) {
       // seleccionar vertice de menor peso
       i = minIndPeso (orla, pesos,tam);
       swap (orla, i, --tam);
       v = orla[tam];
       r++; cor[v] = 2; //visitado
       for (x=g[v]; x!=NULL; x=x->prox){
           if (cor[x->dest] == 0){
              cor[x->dest] = 1; orla[tam++] = x->dest;
              pais[x->dest] = v; 
              pesos[x->dest] = pesos[v] + x->custo;
           }
           else if (cor[x->dest] == 1 && 
                    pesos[v] + x->custo < pesos[x->dest]) {
                    pais[x->dest] = v; 
                    pesos[x->dest] = pesos[v] + x->custo;
           }
       }
    }
    return r;
}

void fromLAdj (GrafoL in, GrafoM out){
    int orig, dest; LAdj x;
    for (orig=0; orig<NV; orig++){
        for (dest=0; dest<NV; dest++)
            out[orig][dest] = 0;
        for (x=in[orig]; x!=NULL; x = x->prox)
            out[orig][x->dest] = x->custo;
    }
}

int minCusto (int c1, int c2){
    if (c1 == 0 || (c2 != 0 && c2 <= c1)) return c2;
    return c1;
}

void FW (GrafoL g, GrafoM out) {
    int i, orig, dest;
    fromLAdj (g, out);
    for (i = 0; i < NV; i++)
        for (orig = 0; orig < NV; orig++)
            if (out[orig][i] != 0)
               for (dest = 0; dest < NV; dest++)
                 if (out[i][dest] != 0)
                   out[orig][dest] = minCusto(out[orig][dest], out[orig][i] + out[i][dest]);
}

void invert_graph (GrafoL g) {
    for (int i = 0; i < NV; i++)
        for (LAdj node = g[i]; node != NULL; node = node->prox)
            node->custo *= -1;
}

int maisLonga (GrafoL g, int or, int p[]){
    
    int pais[NV];
    int pesos[NV];

    int MAX = 10000;
    for (int i = 0; i < NV; i++) pesos[i] = MAX;

    dijkstraSP(g, or, pais, pesos);

    int max = or;
    for (int i = 0; i < NV; i++) {
        if (pesos[i] > pesos[max] && pesos[i] != MAX) {
            max = i;
        }
    }

    int r = max;
    int pos = r;
    int k = 0;

    for (int i = pos; i >= 0; i--) {
        p[i] = pos;
        pos = pais[pos];
    }

    return pesos[max];
}

int excentricidadeV (GrafoL g, int v) {
    GrafoM gm;
    FW(g, gm);

    int max = 0;

    for(int j = 0; j < NV; j++)
        if (gm[v][j] > max) max = gm[v][j];
    
    return max;

}

int excentricidadeTotal_1 (GrafoL g) {
    GrafoM gm;
    FW(g, gm);

    int max = -1000;

    for (int i = 0; i < NV; i++)
        for(int j = 0; j < NV; j++)
            if (gm[i][j] > max) max = gm[i][j];
    
    return max;
}

int excentricidadeTotal_2 (GrafoL g) {

    int max = -1000, aux;
    for (int i = 0; i < NV; i++) {
        int p[NV] = {0};
        aux = maisLonga(g, i, p);
        if (aux > max) max = aux;
    }

    return max;
}

int ord_top (GrafoL g, int seq[], int ant[]) {

    int indegree[NV];
    for (int i = 0; i < NV; i++) indegree[i] = 0;

    for (int i = 0; i < NV; i++)
        for (LAdj node = g[i]; node != NULL; node = node->prox)
            indegree[node->dest]++;

    int front = 0, rear = 0;
    for (int i = 0; i < NV; i++)
        if (indegree[i] == 0) {
            seq[rear++] = i;
            ant[i] = -1;
        }

    while (front < rear) {
        int curr = seq[front++];
        for (LAdj node = g[curr]; node != NULL; node = node->prox) {
            indegree[node->dest]--;
            if (indegree[node->dest] == 0)
                seq[rear++] = node->dest;
        }
    }

    return front;
}

int excentricidadeTotal_3 (GrafoL g) {

    int ant[NV];
    int OT[NV], e[NV];
    int n = ord_top(g, OT, ant);
    for (int i = 0; i < NV; i++) e[i] = 0;

    int v;
    for (int i = 0; i < n; i++) {
        v = OT[i];
        for (LAdj node = g[v]; node != NULL; node = node->prox) {
            if (e[v] + node->custo > e[node->dest])
                e[node->dest] = e[v] + node->custo;
                ant[node->dest] = v;
        }
    }

    int r = 0, index = -1;
    for (int i = 0; i < NV; i++)
        if (e[i] > r) {
            r = e[i];
            index = i;
        }
    
    int pos = index;
    while (pos != -1) {
        printf("%d\n", pos);
        pos = ant[pos];
    }

    return r;
}

#define RED   1
#define WHITE 0
#define BLUE -1


int PrimMST (GrafoL g, int cost[], int ant[]) {
    int orla[NV], color[NV] = {0};
    int size = 0;

    int curr = 0;
    color[curr] = BLUE ; cost[curr] = 0, ant[curr] = -1;

    int i;    
    orla[size++] = curr;

    while (size > 0) {

        // printf("Conteúdo da orla: ");
        // for (int i = 0; i < size; i++) printf("[%d; cost: %d] ", orla[i], cost[orla[i]]);
        // printf("\n");

        i = minIndPeso(orla, cost, size);
        swap(orla, i, --size);

        curr = orla[size]; // utiliza-se o size, visto que foi trocado
        color[size] = RED; 

        // printf("get: %d; cost: %d\n", curr, cost[curr]);

        for (LAdj node = g[curr]; node != NULL; node = node->prox) {
            if (color[node->dest] == WHITE || (color[node->dest] == BLUE && cost[node->dest] > node->custo)) {
                ant[node->dest] = curr;
                cost[node->dest] = node->custo;
                if (color[node->dest] == WHITE) {
                    color[node->dest] = BLUE;
                    orla[size++] = node->dest;
                    // printf("add: %d; cost: %d\n", node->dest, node->custo);
                }
            } 
        }
        // printf("\n");
    }
}

int arvore_geradora_custo_minimo (GrafoL g) {
    int ant[NV];
    for (int i = 0; i < NV; i++) ant[i] = -2;
    int cost[NV];

    int max = 1000;
    for (int i = 0; i < NV; i++) cost[i] = max;

    PrimMST(g, cost, ant);

    printf("\n ");
    for (int i = 0; i < NV; i++) printf("%d ", i);
    printf("\n");
    for (int i = 0; i < NV; i++) if (ant[i] != -2) printf("%d ", ant[i]); else printf("_ ");
    printf("\n\n");
}

// nao functiona com ciclos
void print_all(GrafoL g, int c, int n) {
    printf("%d ", c);

    if (c == n) {
        return;
    }

    for (LAdj node = g[c]; node != NULL; node = node->prox) {
        print_all(g, node->dest, n);
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

    fromMat(gm, gl);

    int p[NV];
    for (int i = 0; i < NV; i++) p[i] = 1000;
    int r = maisLonga(gl, 0, p);
    printf("Mais longa: %d\n", r);
    
    for (int i = 0; i < NV;  i++) if (p[i] != 1000) printf("%d ", p[i]);
    printf("\n");

    printf("[em 0] v1: %d\n", excentricidadeV(gl, 0));
    arvore_geradora_custo_minimo(gl);

    // ------------------

    double time_spent = 0.0;
    clock_t begin_1 = clock();

    printf("%d :: ", excentricidadeTotal_1(gl));

    clock_t end_1 = clock();
    time_spent += (double)(end_1 - begin_1) / CLOCKS_PER_SEC;
    printf("The elapsed time is %lf \n", time_spent);

    // ------------------

    time_spent = 0.0;
    clock_t begin_2 = clock();

    printf("%d :: ", excentricidadeTotal_2(gl));

    clock_t end_2 = clock();
    time_spent += (double)(end_2 - begin_2) / CLOCKS_PER_SEC;
    printf("The elapsed time is %lf \n", time_spent);

    // ------------------

    time_spent = 0.0;
    clock_t begin_3 = clock();

    printf("%d :: ", excentricidadeTotal_3(gl));

    clock_t end_3 = clock();
    time_spent += (double)(end_3 - begin_3) / CLOCKS_PER_SEC;
    printf("The elapsed time is %lf \n", time_spent);

    // NÃO DÁ PRA ENCONTRAR OS CAMINHOS MAIS LONGOS COM 100% DE CERTEZA COM O DIJKSTRA

    return 0;
}