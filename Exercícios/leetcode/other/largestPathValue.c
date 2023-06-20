#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int dfirstTopSort (char *colors, bool *visited, int edges[][2], int i, int edgesSize, int *edgesColSize, int *check_cycle) {

    int t = 0;

    for (int i = 0; i < edgesColSize[i]; i++) {
        if (!visited[edges[i][1]]) {
            t += dfirstTopSort(colors, visited, edges, i, edgesSize, edgesColSize, check_cycle);
        } else {
            *check_cycle = 0;
        }
    }

    t++;
    visited[i] = true;
    return t;

}

#define WHITE 0
#define GRAY -1
#define BLACK 1

int DF (int edges[4][2], int edgesSize, int *edgesColSize) {

    int visited[edgesSize];
    memset(visited, 0, sizeof(int)*edgesSize);

    int size = 0;
    int stack[edgesSize];
    memset(stack, 0, sizeof(int)*edgesSize);

    int k = 0;
    int seq[edgesSize];

    int check_cycle = 0;
    stack[size] = 0;
    size++;

    while (size) {
        int curr = stack[size-1];
        seq[k++] = curr;
        size--;

        if (visited[curr] != WHITE) {
            check_cycle = 1;
            break;
        } else {
            visited[curr] = GRAY;
        }

        for (int i = curr; i < edgesColSize[curr]; i++)
            if (visited[edges[i][1]] == WHITE)
                stack[size++] = edges[i][1];
            
    }

    for (int i = 0; i < k; i++) {
        printf("%d\n", seq[i]);
    }
}

int largestPathValue_3 (char * colors, int edges[][2], int edgesSize, int* edgesColSize) {
    
    int check_cycle = 1;
    int count_path = 0;

    int count_colors[26] = {0};
    bool visited[edgesSize];
    memset(visited, 0, sizeof(bool)*edgesSize);

    for (int i = 0; i < edgesSize && check_cycle; i++) {
        if (!visited[i]) {
            count_path += dfirstTopSort(colors, visited, edges, i, edgesSize, edgesColSize, &check_cycle);
        }
    }

    return count_path;
}

// https://leetcode.com/problems/largest-color-value-in-a-directed-graph/solutions/1199642/java-87ms-dp-dfs-clean-solution/
// We can use a simple DFS to reach leaf nodes, and then go back and track the maximum counts for each color.

#define MAXN 100
#define MAXE 200

/* adjacency list */
int adj[MAXN][MAXN];
int count[MAXN][MAXN];
int visited[MAXN];
int done[MAXN];
int c[MAXN][26];
int answer = 0;
char *color;
int cycle = 0;
int n;

void dfs(int node) {
    visited[node] = 1;
    for (int i = 0; i < n; i++) {
        if (adj[node][i]) {
            if (visited[i] && !done[i]) {
                cycle = 1;
                return;
            }
            c[i][color[node] - 'a'] += c[node][color[node] - 'a'];
            dfs(i);
            for (int j = 0; j < 26; j++) {
                c[node][j] = c[node][j] > c[i][j] ? c[node][j] : c[i][j];
            }
        }
    }
    c[node][color[node] - 'a']++;
    answer = answer > c[node][color[node] - 'a'] ? answer : c[node][color[node] - 'a'];
    visited[node] = 0;
    done[node] = 1;
}

int largestPathValue_2(char* colors, int edges[][2], int edgesSize, int* edgesColSize) {
    /* allocate memory for the color array */
    color = malloc(MAXN * sizeof(char));
    if (color == NULL) {
        /* handle the error */
    }

    strcpy(color, colors);
    n = strlen(color);

    /* initialize adjacency list and count array */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            adj[i][j] = 0;
            count[i][j] = 0;
        }
    }

    /* build adjacency list and update count array */
    for (int i = 0; i < edgesSize; i++) {
        adj[edges[i][0]][edges[i][1]] = 1;
        count[edges[i][0]][edges[i][1]]++;
    }

    memset(visited, 0, sizeof(visited));
    memset(done, 0, sizeof(done));
    memset(c, 0, sizeof(c));
    answer = 0;
    cycle = 0;

    for (int i = 0; i < n; i++) {
        if (!done[i]) dfs(i);
    }

    if (cycle) return -1;

    return answer;
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int largestPathValue(char *colors, int **edges, int edgesSize, int *edgesColSize) {
    int n = strlen(colors);

    int *graph = malloc(n * sizeof(int));
    int *deg = malloc(n * sizeof(int));
    memset(deg, 0, n * sizeof(int));
    memset(graph, 0, n * sizeof(int));


    for (int i = 0; i < edgesSize; i++) {
        graph[edges[i][0] * n + edges[i][1]] = 1;
        deg[edges[i][1]]++;
    }

    int *dp = malloc(n * sizeof(int));
    memset(dp, 0, n * sizeof(int));

    int *q = malloc(n * sizeof(int));
    int head = 0, tail = 0;
    int ans = -1;

    for (int i = 0; i < n; i++) {
        if (deg[i] == 0) {
            q[tail++] = i;
            dp[i] = 1;
        }
    }

    int seen = 0;
    while (head < tail) {
        int u = q[head++];
        int val = 0;

        val = max(val, dp[u]);

        ans = max(ans, val);
        seen++;

        for (int v = 0; v < n; v++) {
            if (graph[u * n + v]) {
                dp[v] = max(dp[v], dp[u] + (colors[v] - 'a' == colors[u] - 'a'));
                if (--deg[v] == 0) {
                    q[tail++] = v;
                }
            }
        }
    }

    free(q);
    free(dp);
    free(graph);
    free(deg);

    if (seen < n) return -1;
    return ans;
}


int largestPathValue_4(char* colors, int edges[][2], int edgesSize, int* edgesColSize) {
    
    int count = 0;
    int N = strlen(colors);

    int graph[N][N];
    memset(graph, 0, sizeof(int)*N*N);

    int indegree[N];
    memset(indegree, 0, sizeof(int)*N);

    for (int i = 0; i < 2; i++) {
        graph[edges[i][0]][edges[i][1]] = 1;
        indegree[edges[i][1]]++;
    }

    int dp[N][26];
    memset(dp, 0, sizeof(dp));

    int queue[N]; 
    int front = 0, rear = 0;

    for (int i = 0; i < N; i++)
        if (indegree[i] == 0) {
            queue[rear++] = i;
            dp[i][colors[i] - 'a'] = 1;
        }

    int ans = -1;
    while (front < rear) {
        int curr = queue[front++];

        dp[curr][colors[curr] - 'a']++;
        count++;

        ans = max(ans, dp[curr][colors[curr] - 'a']);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < 26; j++) {
                dp[i][j] = max(dp[i][j], dp[curr][j]);
            }
            indegree[i]--;
            if (indegree[i] == 0) queue[rear++] = i;
            if (count == N) break;
        }
    }

    if (front < rear || count < N) return -1;
    return ans;
    
}

int largestPathValue_90(char *colors, int **edges, int edgesSize, int *edgesColSize) {

    int n = strlen(colors);

    int *deg = malloc(n * sizeof(int));
    memset(deg, 0, n * sizeof(int));

    for (int i = 0; i < edgesSize; i++) {
        deg[edges[i][1]]++;
    }

    int **dp = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        dp[i] = malloc(26 * sizeof(int));
        memset(dp[i], 0, 26 * sizeof(int));
    }

    int *q = malloc(n * sizeof(int));
    int head = 0, tail = 0;
    int ans = -1;

    for (int i = 0; i < n; i++) {
        if (deg[i] == 0) {
            q[tail++] = i;
            dp[i][colors[i] - 'a'] = 1;
        }
    }

    int count = 0;
    int seen = 0;
    while (head < tail) {
        int u = q[head++];
        int val = 0;

        for (int i = 0; i < 26; i++) {
            val = max(val, dp[u][i]);
        }

        ans = max(ans, val);
        seen++;

        for (int j = 0, v = edges[u][1]; j < edgesColSize[u]; j++, v = edges[u++][1]) {
                for (int i = 0; i < 26; i++) {
                    dp[v][i] = max(dp[v][i], dp[u][i] + (i == colors[v] - 'a'));
                }

                if (--deg[v] == 0) {
                    q[tail++] = v;
                }
        }
    }

    free(q);

    for (int i = 0; i < n; i++) {
        free(dp[i]);
    }

    free(dp);
    free(deg);

    if (seen < n) return -1;
    return ans;

}

int main() {

    char colors[] = "bbc";
    int edges[][2] = {{0, 1}, {0, 2}, {1, 2}};
    int result = largestPathValue_4(colors, edges, 3, (int[]){2, 1, 0});
    printf("result = %d\n", result);

    return 0;
}
