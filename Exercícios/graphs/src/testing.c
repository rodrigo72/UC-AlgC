#include "testing.h"
#include <stdlib.h>
#include <math.h>

struct edge {
	int dest;
    int cost;
    struct edge *next;
};

int deep_first_rec (Graph g, int o, int v[]) {
	int count = 1;
	v[o] = 1;
	
	for (EList it = g[o]; it != NULL; it = it->next)
		if (!v[it->dest])
			count += deep_first_rec (g, it->dest, v);

	return count;
}

int travessia_deep_first (Graph g, int o) {
	int visitados[NV] = {0};
	return (deep_first_rec (g, o, visitados));
}

int maiorCL (Graph g) {
	int visitados[NV] = {0};
	int max = 0, c;

	for (int i = 0; i < NV; i++)
		if (visitados[i] == 0) {
			c = deep_first_rec (g, i, visitados);
			if (c > max) max = c;
		}

	return max;
}

#define NIL 0
#define RED 1
#define BLUE 2

// A Bipartite Graph is one whose vertices can be divided into disjoint and 
// independent sets, say U and V, such that every edge has one vertex in U and the 
// other in V.
// (neste caso não resulta pois teria de ser um grafo não orientado)
int biPartite_test (Graph g, int src) {
    int color[NV] = {0};
    color[src] = RED;

    EList aux;
    for (int i = 0; i < NV; i++) {
        aux = g[i];
        while (aux != NULL) {
            if (color[aux->dest] == NIL)
                color[aux->dest] = BLUE;
            biPartite_test(g, aux->dest);
        }
    }
	return 0;
}

#define NV 10

int kahn_algorithm(Graph g) {

	int indegree[NV] = {0};
	for (int i = 0; i < NV; i++) 
		for (EList node = g[i]; node != NULL; node = node->next)
			indegree[node->dest]++;

	int queue[NV];
	int front = 0, rear = 0;
	for (int i = 0; i < NV; i++)
		if (indegree[i] == 0) {
			queue[rear++] = i;
		}

	int t = 0;
	int seq[NV] = {0};
	while (front < rear) {
		int curr = queue[front++];
		seq[t++] = curr;

		for (EList node = g[curr]; node != NULL; node = node->next) {
			if (indegree[node->dest] > 0) indegree[node->dest]--;
			else {
				queue[rear++] = node->dest;
			}
		}
	}
}

// Uma otimização que é costume fazer baseia-se na observação de que nesta última fase as
// variáveis front e t têm o mesmo valor, pelo que o array seq pode ser usado para guardar
// a queue

int kahn_ts(Graph g, int seq[]) {

	int indegree[NV] = {0};
	for (int i = 0; i < NV; i++) 
		for (EList node = g[i]; node != NULL; node = node->next)
			indegree[node->dest]++;

	int t = 0, front = 0, rear = 0;
	for (int i = 0; i < NV; i++)
		if (indegree[i] == 0)
			seq[rear++] = i;

	while (front > rear) {
		int curr = seq[front++];

		for (EList node = g[curr]; node != NULL; node = node->next) {
			indegree[node->dest]--;
			if (indegree[node->dest] == 0) {
				seq[rear++] = node->dest;
			}
		}
	}

	// numero de elementos colocados na sequencia
	return front;
}

#include <stdio.h>

int ord_top (EList graph[], int size, int *queue) {
    
    int indegree[size];
    for (int i = 0; i < size; i++) indegree[i] = 0;

    for (int i = 0; i < size; i++) {
        for (EList node = graph[i]; node != NULL; node = node->next)
            indegree[node->dest]++;
	}

	int front = 0, rear = 0;
	for (int i = 0; i < size; i++) 
		if (indegree[i] == 0) 
			queue[rear++] = i;

	while (front < rear) {
		int curr = queue[front++];

		for (EList node = graph[curr]; node != NULL; node = node->next) {
			indegree[node->dest]--;
			if (indegree[node->dest] == 0) 
				queue[rear++] = node->dest;
		}
	}
    
    return front;

}

int maximumDetonation_2(int bombs[][5], int bombsSize){
    
    EList graph[bombsSize];
    EList ant = NULL;


    for (int i = 0; i < bombsSize; i++) {
        graph[i] = NULL;
        for (int j = 0; j < bombsSize; j++) 
            if (i != j && bombs[i][2] >= sqrt(pow(bombs[i][0] - bombs[j][0], 2) + pow(bombs[i][1] - bombs[j][1], 2))) {
                EList new = malloc(sizeof(struct edge));
                new->dest = j;
				new->cost = 1;
                new->next = graph[i];

                graph[i] = new;
            }
    }

    int queue[bombsSize];
	for (int i = 0; i < bombsSize; i++) queue[i] = -1;
    int k = ord_top(graph, bombsSize, queue);

	int cost[bombsSize];
	for (int i = 0; i < bombsSize; i++) cost[i] = 0;

	for (int i = 0; i < bombsSize; i++) {
		int aux = queue[i];
		// printf("%d\n", aux);
		if (aux == -1) continue;
		for (EList node = graph[aux]; node != NULL; node = node->next) {
			if (cost[aux] + 1 > cost[node->dest]) {
				cost[node->dest] = cost[aux] + 1;
			}
		}
	}

	int max = 0;
	for (int i = 0; i < bombsSize; i++)
		if (cost[i] > max) max = cost[i];

    return max;

}

void dfs (EList child, int *nodesVisited, int *visited) {
    while (child) {
        if(!visited[child->dest]) {
            visited[child->dest] = 1;
            (*nodesVisited)++;
            dfs(child->next, nodesVisited, visited);           
        }
        
        child = child->next;
    }
}

int maximumDetonation (int bombs[][5], int bombsSize){
    
    EList graph[bombsSize];

    for (int i = 0; i < bombsSize; i++) {
        graph[i] = NULL;
        for (int j = 0; j < bombsSize; j++) 
            if (i != j && bombs[i][2] >= sqrt(pow(bombs[i][0] - bombs[j][0], 2) + pow(bombs[i][1] - bombs[j][1], 2))) {
                EList new = malloc(sizeof(struct edge));
                new->dest = j;
				new->cost = 1;
                new->next = graph[i];

                graph[i] = new;
            }
    }

	int visited[bombsSize];
	for (int i = 0; i < bombsSize; i++) visited[i] = 0;

	int nodesVisited, res = -1000;

	for (int i = 0; i < bombsSize; i++) {
        int visited[100] = {0};
        nodesVisited = 0;
        visited[i] = 1;
        nodesVisited++;
        dfs(graph[i], &nodesVisited, visited);
        res = fmax(res, nodesVisited);        
    }
    return res;

}

