#include "longestCycle.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int longestCycle (int* edges, int edgesSize) {
    bool visited[edgesSize];
    memset(visited, 0, sizeof(bool)*edgesSize);

    int queue[edgesSize];
    int front = 0, rear = 0;

    queue[rear] = 0;
    rear++;

    while (front != rear) {
        int curr = queue[front];
        front = (front+1) % edgesSize;

        if (!visited[curr]) {
            printf("%d\n", curr);
            visited[curr] = true;
        }

        if (!visited[edges[curr]]) {
            queue[rear] = edges[curr];
            rear = (rear+1) % edgesSize;
        }
        
    }
    
    return 0;

}