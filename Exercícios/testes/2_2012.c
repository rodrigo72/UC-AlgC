#define N 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct edge {
    int dest;
    int cost;
    struct edge *next;
} *Graph[N];

typedef int GraphM [N][N];

void fromMat (GraphM in, Graph out) {

    for (int i = 0; i < N; i++) {
        out[i] = NULL;
        for (int j = 0; j < N; j++) {
            if (in[i][j] != 0) {
                struct edge *aux = malloc(sizeof(struct edge));
                aux->dest = j;
                aux->cost = in[i][j];
                aux->next = out[i];
                out[i] = aux;
            }
        }
    }

}

// grafos sem peso !
int succN(Graph g, int v, int n) {
    
    int queue[N];
    int distances[N];
    bool visited[N];

    for (int i = 0; i < N; i++) {
        queue[i] = -1;
        distances[i] = -1;
        visited[i] = false;
    }

    int front = 0;
    int rear = 0;

    queue[rear++] = v;
    distances[v] = 0;
    visited[v] = true;

    while (front < rear) {
        int u = queue[front++];

        for (struct edge *node = g[u]; node != NULL; node = node->next) {
            if (!visited[node->dest]) {
                queue[rear++] = node->dest;
                distances[node->dest] = distances[u] + 1;
                visited[node->dest] = true;
            }
        }
    }

    int count = 0;
    for (int i = 0; i < N; i++) {
        if (distances[i] >= 0 && distances[i] <= n) {
            count++;
        }
    }

    return count;
    
}

int ord_top (Graph g, int seq[]) {

    int indegree[N];
    for (int i = 0; i < N; i++) indegree[i] = 0;

    for (int i = 0; i < N; i++)
        for (struct edge *node = g[i]; node != NULL; node = node->next)
            indegree[node->dest]++;

    int front = 0, rear = 0;
    for (int i = 0; i < N; i++)
        if (indegree[i] == 0)
            seq[rear++] = i;

    while (front < rear) {
        int curr = seq[front++];
        for (struct edge *node = g[curr]; node != NULL; node = node->next) {
            indegree[node->dest]--;
            if (indegree[node->dest] == 0)
                seq[rear++] = node->dest;
        }
    }

    return front;
}

int df (Graph g, int i, int comp[], int c) {
    comp[i] = c;
    for (struct edge *node = g[i]; node != NULL; node = node->next) {
        if (comp[node->dest] == -1) {
            df(g, node->dest, comp, c);
        }
    }
}

int componentes_2 (Graph g, int comp[]) {

    int i, c = 0;
    for (int i = 0; i < N; i++) comp[i] = -1;

    for (int i = 0; i < N; i++) {
        if (comp[i] == -1) {
            df(g, i, comp, c);
            c++;
        }
    }

}

int directed_to_undirected (GraphM grafo) {

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (grafo[i][j] == 0 && grafo[j][i] != 0) grafo[i][j] = grafo[j][i];
            else if (grafo[i][j] != 0 && grafo[j][i] == 0) grafo[j][i] = grafo[j][i];

}

int main (void) {

    GraphM gm = {{0, 5, 0, 2, 0, 0, 0, 0, 0, 0} // 0
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
    Graph gl;
    fromMat(gm, gl);

    Graph g;
    for (int i = 0; i < N; i++) {
        g[i] = NULL;
    }

    // Add some edges to the graph
    struct edge *e1 = malloc(sizeof(struct edge));
    e1->dest = 1;
    e1->cost = 2;
    e1->next = NULL;
    g[0] = e1;

    struct edge *e2 = malloc(sizeof(struct edge));
    e2->dest = 2;
    e2->cost = 3;
    e2->next = NULL;
    e1->next = e2;

    struct edge *e3 = malloc(sizeof(struct edge));
    e3->dest = 4;
    e3->cost = 1;
    e3->next = NULL;
    e2->next = e3;

    struct edge *e4 = malloc(sizeof(struct edge));
    e4->dest = 3;
    e4->cost = 1;
    e4->next = NULL;
    g[1] = e4;

    struct edge *e5 = malloc(sizeof(struct edge));
    e5->dest = 2;
    e5->cost = 1;
    e5->next = NULL;
    e4->next = e5;

    // Use the succN function to count the number of vertices at a distance
    // less than or equal to 2 from vertex 0
    int count = succN(g, 0, 2);
    printf("Number of vertices at a distance <= 2: %d\n", count);

    printf("%d\n\n", succN(gl, 0, 5));

    count = 0;
    int comp[N];
    componentes_2(gl, comp);
    for (int i = 0; i < N; i++) {
        if (comp[i] != 0) count++;
        printf("%d\n", comp[i]);
    }

    return 0;
}