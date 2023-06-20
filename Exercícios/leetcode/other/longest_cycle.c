#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int longestCycle (int* edges, int edgesSize) {

    int dist, max = -1;
    int mem[edgesSize][2];
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < edgesSize; j++)
            mem[j][i] = -1;

    for (int i = 0; i < edgesSize; i++) {
        for (int j = i, dist = 0; j != -1; j = edges[j]) {

            int dist_i = mem[j][0];
            int from_i = mem[j][1];

            if (dist_i == -1) {
                mem[j][0] = dist++;
                mem[j][1] = i;
            } else {
                if (from_i == i && dist - dist_i > max) max = dist - dist_i;
                break;
            }

        }
    }

    return max;
}

int longestCycle_2 (int* edges, int edgesSize) {

    bool visited[edgesSize];
    int num_of_nodes[edgesSize];
    int queue[edgesSize];
    int front, rear, count, max;

    for (int i = 0; i < edgesSize; i++) {

        if (edges[i] == -1) continue;

        count = 1;
        front = rear = 0;

        for (int i = 0; i < edgesSize; i++) {
            visited[i] = false;
            num_of_nodes[i] = 0;
        }

        queue[rear] = i;
        rear++;

        while (front != rear) {

            int curr = queue[front];
            front = (front+1) % edgesSize;

            if (edges[curr] == -1) continue;

            if (!visited[curr]) {
                num_of_nodes[curr] = count++;
                visited[curr] = true;
            }

            if (!visited[edges[curr]]) {
                queue[rear] = edges[curr];
                rear = (rear+1) % edgesSize;
            } else {
                int aux = num_of_nodes[curr] - num_of_nodes[edges[curr]] + 1;
                if (aux > max) {
                    max = aux;
                }
            }
        }
    
    }

    return max;

}

int main() {
    int edges[] = {3,3,4,2,3};
    int result = longestCycle(edges, 5);
    printf("\n%d\n", result);
    return 0;
}