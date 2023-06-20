#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int soma (int v[], int a, int b) {
    int r = 0, i;
    for (i=a; i<=b; i++)
        r=r+v[i];

    return r;
}

int maxSoma1 (int v[], int N) {
    int i, j, r=0, m;
    for (i=0; i<N; i++)
        for (j=i; j<N; j++) {
            m = soma(v,i,j);
            if (m>r) r = m;
        }

    return r;
}

int maxSoma2 (int v[], int N) {
    int sum = 0;
    int max = 0;
    int c[N-1];
    for (int i = 0; i < N; i++) {
       
        for(int j = i; j < N; j++) {
            for(int k = j; k < N; k++) {
                sum = soma(v, i, j);
                if (sum > max) max = sum;
            }
        }

        c[i] = max;
        max = 0;
    }

    max = c[0];
    for (int i = 0; i < N; i++) {
        if (c[i] > max) max = c[i];
    }
    return max;
}

int maxSoma4 (int v[], int N) {
    int c[N-1];
    c[0] = v[0];
    int max = c[0];

    for (int i = 1; i < N; i++) {
        if (c[i-1] + v[i] > v[i]) c[i] = c[i-1] + v[i];
        else c[i] = v[i];
    }

    for (int i = 0; i < N; i++) {
        if (c[i] > max) max = c[i];
    }

    return max;
}

int maxSoma6 (int v[], int N) {
    int aux = v[0];
    int r = v[0];
    for (int i = 1; i < N; i++) {
        if (r + v[i] > v[i]) r += v[i];
        else r = v[i];
        if (r > aux) aux = r;
    }
    return aux;
}

int maxSoma7 (int v[], int N) {
    int aux = v[0];
    int r = v[0];
    for (int i = 1; i < N; i++) {
        if (r > 0) r += v[i];
        else r = v[i];
        if (r > aux) aux = r;
    }
    return aux;
}


void printArray (int v[], int N) {
    printf("[");
    for (int i = 0; i < N; i++) {
        printf(" %d", v[i]);
    }
    printf(" ]\n");
}

int crescente (int v[], int N) {
    int i;
    for (i=1; i<N; i++) {
        if (v[i] < v[i-1]) break;
        printf("%d ", i);
    }
    printf("\n");
    return i;
}

int maxcresc (int v[], int N) {
    int r = 1, i = 0, m;
    while (i<N-1) {
        printf("%d: ", i);
        m = crescente (v+i, N-i);
        if (m>r) r = m;
        i++;
    }
    return r;
}

int maxcresc2 (int v[], int N) {
    int r = 1, i = 0, m;
    while (i<N-1) {
        m = crescente (v+i, N-i);
        if (m>r) r = m;
        if (r > N/2) break;
        i++;
    }
    return r;
}


int main() {

    int size = 20;
    int v[size-1];
    srand(time(NULL));

    for (int i = 0; i < size; i++) {
        v[i] = (int) (rand()%52);
    }

    int rand1 = rand()%(size-5) + 5;
    for (int i = 1; i < rand1; i++) {
        v[(int) (rand()%size)] = -(rand()%52);
    }

    // int size = sizeof(v)/sizeof(v[0]);
    printArray(v, size);
    printf("maxSoma versão 1: %d O\n", maxSoma1(v, size));
    printf("maxSoma versão 2: %d O\n", maxSoma2(v, size));
    printf("maxSoma versão 4: %d O\n", maxSoma4(v, size));
    printf("maxSoma versão 6: %d O <- Sem array auxiliar\n", maxSoma6(v, size));
    printf("maxSoma versão 7: %d O\n", maxSoma7(v, size));
    printf("\nmaxCresc versão 1: %d\n", maxcresc(v, size));
    printf("\nmaxCresc versão 2: %d\n", maxcresc(v, size));

    return 0;
}