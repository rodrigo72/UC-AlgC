#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define N 4

int print_solution(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int is_queen_valid (int board[N][N], int row, int col) {
    int i, j;

    for (i = 0; i < col; i++)
        if (board[row][i])
            return 0;

    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j])
            return 0;

    for (i = row, j = col; j >= 0 && i < N; i++, j--)
        if (board[i][j])
            return 0;

    return 1;
}

int solve_n_queen_rec (int board[N][N], int col) {
    if (col >= N)
        return 1;

    for (int i = 0; i < N; i++) {
        if (is_queen_valid(board, i, col)) {
            board[i][col] = 1;
            if (solve_n_queen_rec(board, col+1)) return 1;
            board[i][col] = 0;
        }
    }

    return 0;
}

int solve_n_queen (void) {

    int board[N][N];
    memset(board, 0, sizeof(board));

    int lMove[] = {0, 1,  0, -1};
    int cMove[] = {1, 0, -1,  0};

    if (solve_n_queen_rec(board, 0)) {
        print_solution(board);
        return 1;
    }

    printf("Solution does not exist.\n");
    return 0;
}

int main(void) {
    solve_n_queen ();
    return 0;
}