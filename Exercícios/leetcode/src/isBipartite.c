#include <stdlib.h>
#include <stdbool.h>

#include "isBipartite.h"

// BEST VERSION
// https://leetcode.com/problems/is-graph-bipartite/submissions/

#define WHITE 0
#define RED 1
#define BLUE -1

// Melhor caso: O(1)
// Pior caso: Travessia de todo o grafo ou seja O(Nº de vértices + Nº de arestas)
// O caso médio não é significativamente diference do pior caso
bool isBipartite(int** graph, int graphSize, int* graphColSize) {

    // Inicializar o array de cores a 0 == white
   int color[graphSize];
   for (int i = 0; i < graphSize; i++) color[i] = WHITE;

    // Criar a queue circular (circular de modo a aproveitar melhor o espaço de memória)
   int queue[graphSize];
   int front = 0, rear = 0;

    // percorrer todos os nodes do grafo
   for (int node = 0; node < graphSize; node++) {
       // se já estiver colorido, avança para o próximo
        if (color[node] != WHITE) continue;

        // inicializa-o com vermelho e adiciona à queue
        color[node] = RED;
        queue[rear] = node;
        rear = (rear+1) % graphSize;

        // enquanto a queue não estiver vazia
        while (front != rear) {
            
            // retira elemento da queue (pop)
            int curr = queue[front];
            front = (front+1) % graphSize;

            // nodos adjacentes ao nodo retirado da queue
            for (int i = 0; i < graphColSize[curr]; i++) {
                int aux = graph[curr][i];
                
                // adiciona os elementos acabados de colorir à queue (deste modo)
                if (color[aux] == WHITE) {
                    color[aux] = -color[curr];
                    queue[rear] = aux;
                    rear = (rear+1) % graphSize; // push
                } else if (color[aux] == color[curr]) return false;
            }
        }
   }
   return true;
}

// -----------------------------------------

bool isBipartite_2(int **G, int n, int *cols) {
    int u = 0, v = 0;
    int top = 0;
    int *Q = calloc(n, sizeof(int));
    int *color = calloc(n, sizeof(int));
    int *visited = calloc(n, sizeof(int));
    int *enqueued = calloc(n, sizeof(int));
    
	/* Run a BFS from every node that is still unvisited. */
    for (int i = 0; i < n; i++)
    {
        if (visited[i]) continue;
        color[i] = 1;
        Q[top++] = i;
        
        while (top)
        {
            u = Q[--top];
            enqueued[u] = 0;
            visited[u] = 1;
			
			/* Visit each neighbor of u. */
            for (int j = 0; j < cols[u]; j++)
            {
                v = G[u][j];
                if (visited[v]) continue;
				
				/* If u and v are the same color, then the graph is not bipartite. */
                if (color[u] == color[v]) return false;
				
				/* Else set v to the opposite color of u. */
                color[v] = -color[u];
				
                /* Check if a node has been already enqueued. If we don't check, our Q might have
				 * to hold many nodes multiple times increasing our memory requirement.
				 */
				if (enqueued[v]) continue;
				
                Q[top++] = v;
                enqueued[v] = 1;
            }
        }
    }
    
	return true;
}