#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define N 10
#define MAX 9

void generate_random_array (int arr[]) {
    for (int i = 0; i < N; i++) {
       arr[i] = (rand() % MAX) + 1;
    }
}

void print_array (int arr[]) {
    printf("[ ");
    for (int i = 0; i < N; i++) {
        if (arr[i] != 0)
            printf("%d ", arr[i]);
    }
    printf("]\n");
}

int sum_array (int arr[]) {
    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += arr[i];
    }
    return sum;
}

int make_subset_rec(int arr[], int subset[], int constraint, int n) {

    int sum = sum_array(subset);

    if (sum == constraint)
        return 1;

    for (int i = 0; i < N; i++) {
        if (sum + arr[i] <= constraint && subset[i] == 0) {
            subset[i] = arr[i];
            if (make_subset_rec(arr, subset, constraint, i)) return 1;
            subset[i] = 0; // backtracking
        }
    }

    return 0;
}

int make_subset(int arr[], int constraint) {
    int subset[N];
    for (int i = 0; i < N; i++) subset[i] = 0;

    if (make_subset_rec(arr, subset, constraint, 0)) {
        print_array(subset);
        printf("Sum: %d\n", sum_array(subset));
        return 1;
    }

    printf("Solution does not exist.\n");
    return 0;
}

int main(void) {
    
    int arr[N];
    srand(time(NULL));
    generate_random_array(arr);
    print_array(arr);

    int r = sum_array(arr);
    printf("Sum: %d\n\n", r);

    int constraint = r / 2;
    make_subset(arr, constraint);

    return 0;
}