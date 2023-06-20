#include "adjacency_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// REPRESENTAÇÕES

void add_node(GrafoL out, int i, int j, int c) {
    
    assert(out != NULL && i >= 0 && j >= 0);

    LAdj aux = NULL;
    LAdj list = out[i];

    while (list != NULL) {
        aux = list;
        list = list->prox;
    }

    LAdj new = malloc(sizeof(struct aresta));
    new->dest = j;
    new->custo = c;
    new->prox = NULL;

    if (aux == NULL) {
        out[i] = new;
    } else {
        aux->prox = new;
    }  
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

void fromMat (GrafoM in, GrafoL out) {

    for (int i = 0; i < NV; i++) {
        out[i] = NULL;
        for (int j = 0; j < NV; j++) {
            if (in[i][j] != 0) {
                add_node(out, i, j, in[i][j]);
            }
        }
    }
}

// pior caso: NV^2
void fromMat_2 (GrafoM in, GrafoL out) {

    for (int i = 0; i < NV; i++) {
        out[i] = NULL;
        for (int j = 0; j < NV; j++) {
            if (in[i][j] != 0) {
                LAdj aux = malloc(sizeof(struct aresta));
                aux->dest = j;
                aux->custo = in[i][j];
                aux->prox = out[i];
                out[i] = aux;
            }
        }
    }
}

void inverte (GrafoL in, GrafoL out) {
    LAdj aux, new;

    for (int j = 0; j < NV; j++) out[j] = NULL;

    for (int i = 0; i < NV; i++) {
        aux = in[i];
        while (aux != NULL) {
            new = malloc(sizeof(struct aresta));
            new->dest = i;
            new->custo = aux->custo;
            new->prox = out[aux->dest];
            out[aux->dest] = new;
            aux = aux->prox;
        }
    }
}

int adj (GrafoL g, int o, int d) {
    LAdj aux = g[o];
    while (aux != NULL) {
        if (aux->dest == d)
            return aux->custo;
        aux = aux->prox;
    }
    return 0;
}

// pior caso: NV * ( 2 + a)
int in_degree(GrafoL g) {
    int count[NV], max;
    for (int i = 0; i < NV; i++) count[i] = 0;

    LAdj aux;
    for (int i = 0; i < NV; i++) {
        aux = g[i];
        while (aux != NULL) {
            count[aux->dest]++;
            aux = aux->prox;
        }
    }

    max = count[0];
    for (int i = 1; i < NV; i++) {
        if (count[i] > max) 
            max = count[i];
    }

    return max;
}

int colorOK (GrafoL g, int cor[]) {
    
    LAdj aux;
    for (int i = 0; i < NV; i++)
        for (aux = g[i]; aux != NULL; aux = aux->prox)
            if (cor[i] == cor[aux->dest]) return 0;
        
    return 1;
}

int homomorfOK (GrafoL g, GrafoL h, int f[]) {
    
    LAdj aux;
    for (int i = 0; i < NV; i++)
        for (aux = g[i]; aux != NULL; aux = aux->prox)
            if (!adj(h, f[i], f[aux->dest])) return 0;
    
    return 1;
}

// TRAVESSIAS

int DFRec (GrafoL g, int or, int v[], int p[], int l[]);

int DF (GrafoL g, int or, int v[], int p[], int l[]) {
    int i;
    for (i = 0; i < NV; i++) {
        v[i] = 0;
        p[i] = -1;
        l[i] = -1;
    }

    p[or] = -1; 
    l[or] = 0;
    return DFRec (g,or,v,p,l);
}

int DFRec (GrafoL g, int or, int v[], int p[], int l[]) {
    int i; LAdj a;
    
    i = 1;
    v[or] = -1;
    for (a = g[or]; a != NULL; a = a->prox) 
      if (!v[a->dest]) {
         p[a->dest] = or;
         l[a->dest] = 1 + l[or];
         i += DFRec(g,a->dest,v,p,l);
      }

    v[or]=1;
    return i;
}

int BF (GrafoL g, int or, int v[], int p[], int l[]) {
    int i, x; LAdj a; 
    int q[NV], front, end;

    for (i = 0; i < NV; i++) {
       v[i] = 0;
       p[i] = -1;
       l[i] = -1;
    }

    front = end = 0;
    q[end++] = or; //enqueue
    v[or] = 1; 
    p[or]=-1;l[or]=0;
    i=1;
    while (front != end) {
        x = q[front++]; //dequeue
        for (a = g[x]; a != NULL; a = a->prox)
            if (!v[a->dest]){
                i++;
                v[a->dest]=1;
                p[a->dest]=x;
                l[a->dest]=1+l[x];
                q[end++]=a->dest; //enqueue
            }  
   } 
    return i;
}

void topological_order (GrafoL g, int k) {
    
    // topological order
    // 0 1 2 3 4 5 6 7 8 9
    int t_ord[NV];
    int arestas[NV] = {0};
    int ant[NV];

    for (int i = 0; i < NV; i++) t_ord[i] = MAX;
    
    ant[k] = -1;
    t_ord[k] = 0;
    arestas[k] = 0;

    LAdj aux;
    for (int i = 0; i < NV; i++) {
        aux = g[k];
        while (aux != NULL) {
            if ((aux->custo + t_ord[k]) < t_ord[aux->dest]) {
                t_ord[aux->dest] = aux->custo + t_ord[k];
                arestas[aux->dest] = 1 + arestas[k];
                ant[aux->dest] = k;
            }
            aux = aux->prox;
        }
        k = (k+1) % NV;
    }

    int aux_2;
    int max = -1;
    int max2 = 0;
    int pos = -1;


    for (int i = 0; i < NV; i++) {
        aux_2 = t_ord[i];
        if (aux_2 < 1000 && aux_2 > max) {
            max = aux_2;
            pos = i;
        }
        if (arestas[i] > max2) max2 = arestas[i];
    }

    int count = 0;
    while (count < max2 && pos != -1) {
        printf("%d ", pos);
        pos = ant[pos];
        count++;
    }
    printf("yep %d\n", count);

}

// Bellman-ford algorithm
int maisLonga (GrafoL g, int or, int p[]) {
    
    int new_cost;
    int cost[NV];
    int prev[NV];

    for (int i = 0; i < NV; i++) cost[i] = MAX;
    
    cost[or] = 0;
    prev[or] = -1;

    // for (int i = 0; i < NV; i++)
        for (int j = 0; j < NV; j++)
            if (cost[j] != MAX)
                for (LAdj node = g[j]; node != NULL; node = node->prox) {
                    new_cost = cost[j] + node->custo;
                    if (cost[node->dest] == MAX || new_cost < cost[node->dest]) {
                        cost[node->dest] = new_cost;
                        prev[node->dest] = j;
                    }
                }

    int max = MIN, index = -1;
    for (int i = 0; i < NV; i++) {
        if (cost[i] != MAX && cost[i] > max) {
            max = cost[i];
            index = i;
        }
    }

    int k = 0;
    int pos = index;

    while (pos != -1) {
        printf("%d\n", pos);
        p[k] = pos;
        pos = prev[pos];
        k++;  
    }

    return (k-1) > 0 ? k-1 : 0;

}

int maisLonga_2 (GrafoL g, int or, int p[]) {
    
    int i, r, max = or;
    int vis[NV], prev[NV], dist[NV];

    BF(g, or, vis, prev, dist);

    for (i = 0; i < NV; i++) {
        if (dist[i] > dist[max]) {
            max = i;
        }
    }

    r = dist[max];
    int pos = r;
    int k = 0;

    while (pos != -1) {
        p[k] = pos;
        pos = prev[pos];
        k++;
    }

    return r;

}

int directed_to_undirected (GrafoM grafo) {
    for (int i = 0; i < NV; i++) {
        for (int j = 0; j < NV; j++) {
            if (grafo[i][j] == 0 && grafo[j][i] != 0) grafo[i][j] = grafo[j][i];
            else if (grafo[i][j] != 0 && grafo[j][i] == 0) grafo[j][i] = grafo[j][i];
        }
    }
}

// calcula as componentes ligadas
// preeenche o array c de tal forma que, para quaisquer par de vértices x e y, c[x] == c[y];

int componentes (GrafoL g, int c[]) {

    int i, flag = 1;
    int vis[NV] = {0};
    int prev[NV], dist[NV];

    // all completely zeros
    // routine written directly in assembly and optimized by hand
    // memset(c, 0, sizeof(int) * NV);

    for (int i = 0; i < NV; i++) c[i] = 0;

    for (i = 0; i < NV && flag; i++) {
        if (vis[i] != 0) continue;
        
        BF(g, i, vis, prev, dist);

        for (int j = 0; j < NV && flag; j++) {

            if (vis[j]) {
                
            } else if (c[j] == 0 && flag == 1) flag = 0;

        }

    }

    return i;
}


