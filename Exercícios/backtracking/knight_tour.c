#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void print_solution (int N, int *board) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i*N + j]);
        }
        printf("\n");
    }
}

int is_move_valid (int l, int c, int N, int *board) {
    return (l >= 0 && l < N && c >= 0 && c < N && board[l*N+c] == -1);
}

int backtrack(int l, int c, int *board, int N, int *lMove, int *cMove, int visited) {
    
    if (visited == N*N) {
        return 1;
    }

    for (int i = 0; i < 8; i++) {
        int ln = l + lMove[i];
        int cl = c + cMove[i];
        if (is_move_valid(ln, cl, N, board)) {
            board[ln*N+cl] = visited;
            if (backtrack(ln, cl, board, N, lMove, cMove, visited+1)) return 1;
            else board[ln*N+cl] = -1; // backtracking
        }
    }

    return 0;
} 

int solve (int N) {
    int size = N*N;
    int board[size];
    for (int i = 0; i < size; i++) board[i] = -1;

    int visited = 1;
    board[0] = 0;

    int lMove[8] = { 2, 1, -1, -2, -2, -1,  1,  2 };
    int cMove[8] = { 1, 2,  2,  1, -1, -2, -2, -1 };

    if (backtrack(0, 0, board, N, lMove, cMove, visited)) {
        print_solution(N, board);
        return 1;
    }
    
    printf("Solution does not exist.\n");
    return 0;
}

int main (void) {
    solve(8);
    return 0;
}