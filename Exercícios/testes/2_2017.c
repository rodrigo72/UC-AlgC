#define N 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct edge {
    int dest;
    struct edge *next;
} *Adjlist;

typedef Adjlist Graph[N];

typedef int GraphM [N][N];

void fromMat (GraphM in, Graph out) {

    for (int i = 0; i < N; i++) {
        out[i] = NULL;
        for (int j = 0; j < N; j++) {
            if (in[i][j] != 0) {
                struct edge *aux = malloc(sizeof(struct edge));
                aux->dest = j;
                aux->next = out[i];
                out[i] = aux;
            }
        }
    }

}

void print_adjacency_list (Graph g) {
    Adjlist list;
    for (int i = 0; i < N; i++) {
        printf("Vertex %d\n", i);
        list = g[i];
        while (list != NULL) {
            printf("[%d] ", list->dest);
            list = list->next;
        }
        printf("\n");
    }
}

#define WHITE 0
#define RED   1
#define GREEN 2

int bicolor (Graph g, int color[]) {
    
    for (int v = 0; v < N; v++) color[v] = WHITE;
    
    int queue[N];
    int front = 0, rear = 0;

    for (int i = 0; i < N; i++) {

        if (color[i] != WHITE) continue;

        queue[rear] = i;
        color[i] = RED;
        rear = (rear+1) % N;

        while (front != rear) {

            int curr = queue[front];
            front = (front+1) % N;

            for (struct edge *node = g[curr]; node != NULL; node = node->next) {
                if (color[node->dest] == WHITE) {
                    color[node->dest] = color[curr] == RED ? GREEN : RED;
                    queue[rear] = node->dest;
                    rear = (rear+1) % N;
                } else if (color[curr] == color[node->dest])
                    return 0;
            }
        }
    }

    return 1;
}

int check_combination(Graph g, int comb[]) {
    return 0;
}


int make_combinations(Graph g, int arr[], int size, int index, int* combination) {
  // If the current index is greater than the size of the array,
  // we have reached the end of the array, so we can print the current combination
  if (index > N) {
    // for (int i = 0; i < N; i++) {
    //   printf("%d ", combination[i]);
    // }
    // printf("\n");
    return check_combination(g, combination);
  }

  // Generate all possible combinations by selecting the current element and
  // recursively calling the function on the remaining elements
  for (int i = 0; i < N; i++) {
    combination[index] = arr[i];
    make_combinations(g, arr, size, index + 1, combination);
  }

}

int k_color (Graph g, int k) {

    int colors[N];
    int combination[N];
    for (int i = 0; i < N; i++) {
        colors[i] = i % k;
    }

    return make_combinations(g, colors, N, 0, combination);    

}

int main(void) {

    Graph gl;
    GraphM gm = {{0, 5, 0, 2, 0, 0, 0, 0, 0, 0} // 0
                ,{0, 0, 0, 0, 8, 0, 0, 0, 0, 0} // 1
                ,{0, 0, 0, 0, 0, 2, 0, 0, 0, 2} // 2
                ,{0, 0, 0, 0, 0, 0, 2, 0, 0, 0} // 3
                ,{0, 0, 0, 0, 0, 0, 0, 0, 2, 0} // 4
                ,{0, 0, 0, 0, 0, 0, 0, 0, 8, 0} // 5
                ,{0, 0, 0, 0, 0, 0, 0, 1, 0, 0} // 6
                ,{0, 0, 0, 2, 7, 0, 0, 0, 0, 0} // 7
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 4} // 8
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 9
                };
    
    
    fromMat(gm, gl);
    // print_adjacency_list(gl);

    k_color(gl, 2);

    int color[N];
    int r = bicolor(gl, color);
    printf("%d\n", r);

    return 0;
}