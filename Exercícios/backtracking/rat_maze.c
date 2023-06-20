#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define N 4

int is_move_valid (int maze[N][N], int l, int c) {
    return (l >= 0 && c >= 0 && l < N && c < N && maze[l][c] == 1);
}

int print_solution (int sol[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", sol[i][j]);
        }
        printf("\n");
    }
}

int solve_maze_rec (int maze[N][N], int sol[N][N], int *lMove, int *cMove, int l, int c) {
    
    if (l == N-1 && c == N-1 && sol[l][c] == 1) {
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        int next_l = l + lMove[i]; 
        int next_c = c + cMove[i];

        if (is_move_valid(maze, next_l, next_c)) {
            sol[next_l][next_c] = 1;
            if (solve_maze_rec(maze, sol, lMove, cMove, next_l, next_c)) return 1;
            sol[next_l][next_c] = 0;
        }
    }

    return 0;
}

int solve_maze(int maze[N][N]) {

    int sol[N][N] = {{ 0, 0, 0, 0 },
                     { 0, 0, 0, 0 },
                     { 0, 0, 0, 0 },
                     { 0, 0, 0, 0 }};

    int lMove[] = {0, 1,  0, -1};
    int cMove[] = {1, 0, -1,  0};

    sol[0][0] = 1;

    if (solve_maze_rec(maze, sol, lMove, cMove, 0, 0)) {
        print_solution(sol);
        return 1;
    }

    printf("Solution does not exist.\n");
    return 0;
    
}

int main(void) {

    int maze[N][N] = {{ 1, 0, 0, 0 },
                      { 1, 1, 0, 1 },
                      { 0, 1, 0, 0 },
                      { 1, 1, 1, 1 }};

    solve_maze(maze);

    return 0;
}