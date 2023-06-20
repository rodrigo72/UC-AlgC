#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

struct btree {
  int info;
  int weight;
  struct btree *left;
  struct btree *right;
};

typedef struct btree* Btree;

int get_num_nodes (Btree t) {
    if (t == NULL) return 0;
    return 1 + get_num_nodes(t->right) + get_num_nodes(t->left);
}

void update_weights_arr (int pesos[], int x, int n) {
    while (x >= 0) {
        pesos[x] += n;
        x = (x-1)/2;
    }
}

void calculate_weights (Btree t, int pesos[], int x) {

    if (t == NULL) return;

    if (t->left != NULL && t->right != NULL) {
        int index = (x-1)/2;
        update_weights_arr(pesos, x, 2);
    } else if (t->left != NULL || t->right != NULL) {
        update_weights_arr(pesos, x, 1);
    } 

    calculate_weights(t->left,  pesos, 2*x+1);
    calculate_weights(t->right, pesos, 2*x+2);
}

int check_weight_rec (Btree t, int pesos[], int x) {
    if (t == NULL) return 0;

    if (t->weight == pesos[x]) {
        return check_weight_rec(t->left, pesos, 2*x+1) + check_weight_rec(t->right, pesos, 2*x+2);
    } else {
        return 1;
    }
}

int check_weight (Btree t) {

    int N = get_num_nodes(t);
    int pesos[N];
    for (int i = 0; i < N; i++) pesos[i] = 1;

    calculate_weights (t, pesos, 0);
    check_weight_rec (t, pesos, 0);

}

int check_weight_2 (Btree t) {
    if (t == NULL) return 1;
    if (get_num_nodes(t) != t->weight)
        return 0;
    return (check_weight_2(t->left) && check_weight_2(t->right));
}

#define N 10

struct edge {
    int dest;
    struct edge *next;
};

typedef struct edge *GraphL[N];

int shortestBy (GraphL g, int s, int d, int i) {

    int found = 0;
    int visited[N] = {0};
    int prev[N], cost[N] = {0};

    int queue[N];
    int front = 0, rear = 0;

    visited[s] = 1, prev[s] = -1, cost[s] = 0;
    queue[rear++] = s;

    while (front < rear) {
        int curr = queue[front++];
        for (struct edge *node = g[curr]; node != NULL; node = node->next) {
            if (!visited[node->dest]) {
                visited[node->dest] = 1;
                prev[node->dest] = curr;
                cost[node->dest] = cost[curr] + 1;
                queue[rear++] = node->dest;

                if (node->dest == d) {
                    found = 1;
                    front = rear;
                    break;
                }
            }
        }
    }

    if (found) {
        int pos = d;
        while (pos != -1) {
            if (prev[pos] == i) return 1;
            pos = prev[pos];
        }
        return 0;
    } else {
        return -1;
    }

}

int main (void) {

    check_weight(NULL);

    return 0;
}