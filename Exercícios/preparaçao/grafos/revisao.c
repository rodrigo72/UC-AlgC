#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define N 10
#define MAX 10000

struct edge {
    int dest, cost;
    struct edge *next;
};

typedef struct edge *graph[N];

// Algoritmo Bellman-Ford

void bellmanford (graph g, int o, int cost[], int prev[]) {
    
    for (int i = 0; i < N; i++) {
        prev[i] = -1;
        cost[i] = MAX;
    }
    cost[o] = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (cost[j] != MAX) 
                for (struct edge *node = g[j]; node != NULL; node = node->next) {
                    int newcost = cost[j] + node->cost;
                    if (cost[node->dest] == MAX || newcost < cost[node->dest]) {
                        cost[node->dest] = newcost;
                        prev[node->dest] = j;
                    }
                }

}

// Algoritmo de Floyd-Warshal
// Caminho mais curto entre todos os pares de vértices

void floydwarshal (graph gl, int gm[N][N]) {

    // inicialização de gm
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            gm[i][j] = MAX;
        for (struct edge *node = gl[i]; node != NULL; node = node->next)
            gm[i][node->dest] = node->cost;
    }

    // adição de arestas
    for (int i = 0; i < N; i++)
        for (int orig = 0; orig < N; orig++)
            if (gm[orig][i] != MAX)
                for (int dest = 0; dest < N; dest++)
                    if (gm[i][dest] != MAX) {
                        int cost = gm[orig][i] + gm[i][dest];
                        if (gm[orig][dest] == MAX || gm[orig][dest] > cost)
                            gm[orig][dest] = cost;
                    }
}

int *floydwarshal_2 (int g[N][N]) {

    // int dp[N][N][N];
    // d[k][i][j] = shortest path from i to routing through nodes {0, 1, ..., k-1, k}

    // starting with k = 0, then k = 1, etc. This gradually builds up the optimal solution routing routing
    // through 0, then through 0 and 1, etc. until N-1 which stores the APSP solution
    
    // In the beginning the optimal solution between i and j is simply the distance in the adjacency matrix
    // Otherwise:
    // dp[k][i][j] = min(dp[k-1][i][j], dp[k-1][i][k] + dp[k-1][k][j])
    // Explanation:
    // dp[k-1][i][j] => best distance from i to j with values routing through {0, .., k-1}
    // dp[k-1][i][k] => go from i to k  (routing through nodes {0, ..., k-1})
    // dp[k-1][k][j] => go from k to j  (routing through nodes {0, ..., k-1})

    // It is possible to reduce a dimension (O(V^2) memory), since its always used k-1 (the last computed value)
    // No longer involves the k dimension:
    int dp[N][N]; // cost from i to j
    int next[N][N]; // path from i to j

    // 'next' for path reconstruction
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            dp[i][j] = g[i][j];
            if (g[i][j] != MAX)
                next[i][j] = j;
        }

    for (int k = 0; k < N; k++) // gradually build up the best solutions
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (dp[i][k] + dp[k][j] < dp[i][j]) {
                    dp[i][j] = dp[i][k] + dp[k][j];
                    next[i][j] = next[i][k];
                }
    
    // reconstruction of the path
    int *path = malloc(sizeof(int) * N);
    int start, end, i, index = 0;
    // check the distance between the start and the end
    if (dp[start][end] == MAX) return path;

    for (i = start; i != end; i = next[i][end]) {
        path[index] = i;
        index++;
    }

    path[index] = end;
    return path;
}

void topological_order (graph g, int *size, int queue[]) {

    // Calcular o indegree de cada nodo
    int indegree[N] = {0};
    for (int i = 0; i < N; i++)
        for (struct edge *node = g[i]; node != NULL; node = node->next)
            indegree[node->dest]++;

    int prev[N];
    int front = 0, rear = 0;

    // Inserir na queue/ sequência os nodos com indegree igual a zero (e marcar o seu antecessor como -1)
    for (int i = 0; i < N; i++)
        if (indegree[i] == 0) {
            queue[rear++] = i;
            prev[i] = -1;
        }
    
    // diminuir o indegree dos nodos vizinhos, inseri-los na queue caso fiquem com indegree igual a zero
    int i = 0;
    while (front < rear) {
        int curr = queue[front++];
        for (struct edge *node = g[curr]; node != NULL; node = node->next) {
            indegree[node->dest]--;
            if (indegree[node->dest] == 0)
                queue[rear++] = node->dest;
        }
    }

    (*size) = front;

}

// Tarjan's Algorithm
// Strongly Connected Components 
// : self contained cycles within a directed graph where every vertex in a given cycle can reach every
//   other vertex in the same cycle

void dfs (graph g, int ids[], int low_link[], int stack[], int on_stack[], int *size, int *id, int *count, int o) {
    
    int i;
    stack[*size] = o;
    on_stack[o] = 1;
    ids[o] = low_link[o] = (*id)++;

    for (struct edge *node = g[o]; node != NULL; node = node->next) {
        if (ids[node->dest] == -1) 
            dfs(g, ids, low_link, stack, on_stack, size, id, count, node->dest);
        if (on_stack[node->dest]) 
            low_link[o] = low_link[o] > low_link[node->dest] ? low_link[node->dest] : low_link[o];
    }

    // check if its in the start of a scc
    if (ids[o] == low_link[o]) {
        while ((*size) >= 0) {
            i = stack[*size];
            (*size)--;
            on_stack[i] = 0;
            low_link[i] = ids[o];
            if (stack[i] == o) break;
        }
        (*count)++;
    }
}

void tarjan (graph g) {

    int id = 0;
    int scc_count = 0;

    int ids[N];
    int low_link[N] = {0};
    for (int i = 0; i < N; i++) ids[i] = -1;

    int stack[N];
    int on_stack[N] = {0};
    int size = 0;

    for (int i = 0; i < N; i++) {
        if (ids[i] == -1)
            dfs(g, ids, low_link, stack, on_stack, &size, &id, &scc_count, i);
    }
}

int longest_path (graph g) {

    int queue[N], cost[N] = {0};
    int size = 0;
    topological_order(g, &size, queue);

    for (int i = 0; i < size; i++) {
        int curr = queue[i];
        for (struct edge *node = g[curr]; node != NULL; node = node->next) {
            if (cost[curr] + node->cost > cost[node->cost])
                cost[node->cost] = cost[curr] + node->cost;
        }
    }

    int max = 0;
    for (int i = 0; i < N; i++)
        if (cost[i] > max) max = cost[i];

    return max;

}

#define INITIAL  0
#define VISITED  1
#define FINISHED 2

int cc_dfs (graph g, int state[], int i) {
    state[i] = VISITED;
    for (struct edge *node = g[i]; node != NULL; node = node->next) {
        if (state[node->dest] == INITIAL)
            cc_dfs(g, state, node->dest);
        else if (state[node->dest] == VISITED)
            return 1;
    }

    state[i] = FINISHED;

    return 0;
}

int check_cycle (graph g) {

    int state[N] = {0};

    for (int i = 0; i < N; i++) {
        if (state[i] == INITIAL) {
            if (cc_dfs(g, state, i))
                return 1;
        }
    }

    return 0;
    
}

typedef struct bst {
    int key, info;
    struct bst *left, *right;
} *BST;

typedef struct avl {
    int bal;
    int key, info;
    struct avl *left, *right;
} *AVLTree;

int main (void) {

    return 0;
}