#include "chaining.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _nodo {
    char *chave; int ocorr;
    struct _nodo *prox;
};

static Nodo *new_node (char *s) {
    if (s == NULL) return NULL;

    Nodo *nodo = (Nodo *) malloc(sizeof(Nodo));
    nodo->chave = s;
    nodo->ocorr = 1;
    nodo->prox = NULL;

    return nodo;
}

void print_ht (THash t) {
    printf("\n-----\nstart\n-----\n");
    for (int i = 0; i < Size; i++) {
        printf("%d ", i);
        for (Nodo *ptr = t[i]; ptr != NULL; ptr = ptr->prox) {
            printf("-> [ %s : %d ] ", ptr->chave, ptr->ocorr);
        }
        printf("\n");
    }
    printf("-----\nend\n-----\n\n");
}

void initEmpty (THash t) {
    for (int i = 0; i < Size; i++) {
        t[i] = NULL;
    }
}

void add (char *s, THash t) {
    unsigned hashed = hash(s) % Size;
    Nodo *ptr, *ant = NULL;

    for (ptr = t[hashed]; ptr != NULL; ptr = ptr->prox) {
        if (!strcmp(s, ptr->chave)) {
            ptr->ocorr++;
            return;
        }
        ant = ptr;
    }

    Nodo *new = new_node(s);
    if (ant != NULL) {
        ant->prox = new;
    } else {
        t[hashed] = new;
    }
}

int lookup (char *s, THash t) {
    unsigned hashed = hash(s) % Size;

    for (Nodo *ptr = t[hashed]; ptr != NULL; ptr = ptr->prox) {
        if (!strcmp(s, ptr->chave)) {
            return ptr->ocorr;
        }
    }
    return 0;
}

void remove_element (char *s, THash t) {
    unsigned hashed = hash(s) % Size;
    Nodo *ptr, *ant = NULL;
    for (ptr = t[hashed]; ptr != NULL; ptr = ptr->prox) {
        if (!strcmp(s, ptr->chave)) {
            if (ant != NULL) {
                ant->prox = ptr->prox;
            } else {
                t[hashed] = NULL;
            }
            free(ptr);
            return;
        }
        ant = ptr;
    }
}