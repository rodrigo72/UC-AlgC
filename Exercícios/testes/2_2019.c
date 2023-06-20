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
        for (int j = 0; j < N; j++)
            if (in[i][j] != 0) {
                struct edge *aux = malloc(sizeof(struct edge));
                aux->dest = j;
                aux->cost = in[i][j];
                aux->next = out[i];
                out[i] = aux;
            }
    }

}

int simplePath (Graph g, int v[], int k) {
    
    bool found;
    bool visited[N];
    for (int i = 0; i < N; i++) visited[i] = false;

    for (int i = 0; i < k-1; i++) {

        found = false;
        int u = v[i];
        int w = v[i+1];

        for (struct edge *node = g[u]; node != NULL; node = node->next) {
            if (w == node->dest) {
                found = true;
                break;
            }
        }

        if (found == false) return 0;
        if (visited[w] == true) return 0;
        visited[w] = true;
    }

    return 1;
}

void DF (GraphM g, int s, int *visited) {
    
    visited[s] = 1;
    for (int i = 0; i < N; i++)
        if (g[s][i] != 0 && !visited[i]) {
            DF(g, i, visited);
        }
}

int count_reachable (GraphM g, int s) {
    int count = 0;
    int visited[N] = {0};

    DF(g, s, visited);

    for (int i = 0; i < N; i++)
        if (visited[i] != 0) count++;

    return count;
}

typedef struct data {
    int max;
    int index;
} *Data;

Data bfs (Graph g, int v) {
    int dist[N];
    for (int i = 0; i < N; i++) dist[i] = -1;

    int queue[N];
    int front = 0, rear = 0;

    dist[v] = 0;
    queue[rear++] = v;

    while(front < rear) {
        int curr = queue[front++];

        for (struct edge *node = g[curr]; node != NULL; node = node->next)
            if (dist[node->dest] == -1) {
                queue[rear++] = node->dest;
                dist[node->dest] = dist[curr] + 1;
            }
    }

    int max = 0, index = -1;
    for (int i = 0; i < N; i++)
        if (dist[i] > max) {
            max = dist[i];
            index = i;
        }

    Data new = malloc(sizeof(Data));
    new->max = max;
    new->index = index;

    return new;
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

// maior distância entre esse vértice e qualquer outro vértice
int excentricity_20 (Graph g, int v) {

    int seq[N];
    int k = ord_top(g, seq);

    int cost[N] = {0};
    for (int i = 0; i < k; i++) {
        int curr = seq[i];
        for (struct edge *node = g[curr]; node != NULL; node = node->next) {
            if (cost[curr] + node->cost > cost[node->dest]) {
                cost[node->dest] = cost[curr] + node->cost;
            }
        }
    }

    int result = 0;
    for (int i = 0; i < N; i++)
        if (cost[i] > result) result = cost[i];

    return result;

}

void BF (Graph g, int or, int *v, int *prev, int *dist) {
    
    for (int i = 0; i < N; i++) {
       v[i] = 0;
       prev[i] = -1;
       dist[i] = -1;
    }

    int queue[N];
    int front = 0, rear = 0;
    queue[rear++] = or;
    v[or] = 1;
    dist[or] = 0;
    prev[or] = -1;

    int i = 0;
    while (front < rear) {
        int curr = queue[front++];

        for (struct edge *node = g[curr]; node != NULL; node = node->next) {
            if (!v[node->dest]) {
                i++;
                v[node->dest] = 1;
                prev[node->dest] = curr;
                dist[node->dest] = 1 + dist[curr];
                queue[rear++] = node->dest;
            }
        }
    }

}

int excentricity (Graph g, int or) {
    
    int MIN = -100000;
    int MAX =  100000;
    int new_cost;
    int cost[N];

    for (int i = 0; i < N; i++) cost[i] = MAX;
    
    cost[or] = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (cost[j] != MAX)
                for (struct edge *node = g[j]; node != NULL; node = node->next) {
                    new_cost = cost[j] + node->cost;
                    if (cost[node->dest] == MAX || new_cost < cost[node->dest]) {
                        cost[node->dest] = new_cost;
                    }
                }

    int max = MIN, index = -1;
    for (int i = 0; i < N; i++) {
        if (cost[i] != MAX && cost[i] > max) {
            max = cost[i];
        }
    }

    return max;    

}

int excentricity_aaa (Graph g, int or) {
    int i, r, max = or;
    int vis[N], prev[N], dist[N];

    BF(g, or, vis, prev, dist);

    for (i = 0; i < N; i++) {
        if (dist[i] > dist[max]) {
            max = i;
        }
    }

    r = dist[max];
    return r;
}

int excentricity_2(Graph graph, int start_node) {
  // Initialize a queue for the breadth-first search

    int queue[N];
    int front = 0, rear = 0;
  // Initialize an array to keep track of the distances from the starting node
  // to all other nodes in the graph
  int dist[N];
  for (int i = 0; i < N; i++) {
    dist[i] = 1000000;
  }
  dist[start_node] = 0;

  // Initialize an array to keep track of which nodes have been visited
  int visited[N];
  for (int i = 0; i < N; i++) {
    visited[i] = 0;
  }
  visited[start_node] = 1;

  // Add the starting node to the queue
    queue[rear++] = start_node;

  // Perform the breadth-first search
  while (front < rear) {
    // Get the next node from the queue
    int current_node = queue[front++];

    // Visit each unvisited neighbor of the current node
    for (struct edge *node = graph[current_node]; node != NULL; node = node->next) {
      int neighbor = node->dest;
      if (!visited[neighbor]) {
        // Update the distance from the starting node to the neighbor
        dist[neighbor] = dist[current_node] + 1;
        // Mark the neighbor as visited
        visited[neighbor] = 1;
        // Add the neighbor to the queue
        queue[rear++] = neighbor;
      }
    }
  }

  // Find the longest distance from the starting node to any other node
  int longest_distance = 0;
  for (int i = 0; i < N; i++) {
    if (dist[i] != 1000000 && dist[i] > longest_distance) {
      longest_distance = dist[i];
    }
  }

  return longest_distance;
}

int main() {
    Graph gl;
    GraphM gm = {{0, 5, 0, 2, 0, 0, 0, 0, 0, 0} // 0
                ,{0, 0, 0, 0, 8, 0, 0, 0, 0, 0} // 1
                ,{0, 0, 0, 0, 0, 2, 0, 0, 0, 2} // 2
                ,{0, 0, 0, 0, 0, 0, 2, 0, 0, 0} // 3
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 4
                ,{0, 0, 0, 0, 0, 0, 0, 0, 8, 0} // 5
                ,{0, 0, 0, 0, 0, 0, 0, 1, 0, 0} // 6
                ,{0, 0, 0, 0, 7, 0, 0, 0, 0, 0} // 7
                ,{0, 0, 0, 0, 3, 0, 0, 0, 0, 4} // 8
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 9
               };
        
    fromMat(gm, gl);

    Graph g = {NULL};
    
    g[0] = malloc(sizeof(struct edge));
    g[0]->dest = 1;
    g[0]->cost = 1;
    g[0]->next = malloc(sizeof(struct edge));
    g[0]->next->dest = 3;
    g[0]->next->cost = 1;
    g[0]->next->next = NULL;
    g[1] = malloc(sizeof(struct edge));
    g[1]->dest = 2;
    g[1]->cost = 1;
    g[1]->next = NULL;
    g[2] = malloc(sizeof(struct edge));
    g[2]->dest = 3;
    g[2]->cost = 1;
    g[2]->next = NULL;
    g[3] = NULL;

    int v[] = {0, 1, 2, 3};
    int k = 4;
    if (simplePath(g, v, k)) printf("The path is simple\n");
    else printf("The path is not simple\n");

    int v2[] = {0, 1, 2, 3, 0};
    int k2 = sizeof(v2) / sizeof(v2[0]);
    if (simplePath(g, v2, k2)) printf("The path is simple\n");
    else printf("The path is not simple\n");

    int v3[] = {0, 3, 6, 7};
    int k3 = sizeof(v3) / sizeof(v3[0]);
    if (simplePath(gl, v3, k3)) printf("The path is simple\n");
    else printf("The path is not simple\n");

    int src = 6;
    printf("Reachable nodes from %d: %d\n", src, count_reachable(gm, src));
    
    int src_2 = 0;
    printf("Excentricity of %d: %d\n", src_2, excentricity(gl, src));

    return 0;
}