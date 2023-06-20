#include "min_heap.h"

#include <stdlib.h>
#include <stdio.h>

#define COUNT 10

// Uma alternativa para armazenar um dicionário onde existe uma relação de ordem entre
// as chaves são árvores binárias de procura (ordenadas pela chave)

// Uma árvore binária de procura diz-se semi-completa se todos os níveis da árvore estão
// completos, com a possível excessão do último, que pode estar parcialmente preenchido

// O pior caso é correspontende à áltura da árvore
// Entre N (lista) e log2(N) (perfeitamente equilibrada)
int update (BST *a, int k, int i) {
    int u = 0;

    while (*a != NULL && (*a)->key != k) {
        // valores menores à esquerda e maiores à direita do nodo atual
        if ((*a)->key > k)
            a = &((*a)->left);
        else
            a = &((*a)->right);
    }

    if (*a == NULL) { // key não existe + informação adicionada
        *a = (BST) malloc (sizeof(struct bst));
        (*a)->key = k;
        (*a)->info = i;
        (*a)->left = (*a)->right = NULL;
        u = 1;
    } else {          // key existe + informação adicionada
        (*a)->info = i;
    }

    return u;
}

int lookup (BST a, int k, int *i) {
    int found = 1;

    while (a != NULL && a->key != k) {
        if (a->key > k)
            a = a->left;
        else
            a = a->right;
    }

    if (a != NULL) *i = a->info;
    else found = 0;

    return found;
}

void print_bst (BST a, int space) {
    if (a == NULL) return;

    space += COUNT;

    print_bst(a->right, space);

    printf("\n");
    for (int i = COUNT; i < space; i++) printf(" ");
    printf("%d\n", a->info);

    print_bst(a->left, space);

}

int _print_t(AVLTree tree, int is_left, int offset, int depth, char s[20][255]){
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", tree->info);

    int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif

    return left + width + right;
}

void print_t(AVLTree tree) {
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(tree, 0, 0, 0, s);

    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}

// Insersão que preserva o balanceamento da árvore binária de procura
// De forma a atingir isso, guarda-se em cada nodo da árovre um novo campo
// - bal - que traduz o atual estado de balanceamento da árvore que aí se inicia.

// Esse campo poderá ter um de três valores que correspondem à diferença entre
// as alturas da sub-árvore esquerda e da direita:

#define LEFT 1  // quando a sub-árvore da esquerda é mais alta que a da direita
#define BAL 0   // quando as duas sub-árvores têm a mesma altura  
#define RIGHT -1// quando a sub-árvore da direita é mais alta que a da esquerda


AVLTree rotateRight (AVLTree a) {
    AVLTree b = a->left;
    a->left = b->right;
    b->right = a;
    return b;
}

AVLTree rotateLeft (AVLTree b) {
    AVLTree a = b->right;
    b->right = a->left;
    a->left = b;
    return a;
}

AVLTree fixRight (AVLTree a) {

    AVLTree b, c;
    b = a->right;

    if (b->bal == RIGHT) {
        a->bal = b->bal = BAL;
        a = rotateLeft(a);
    } else {
        c = b->left;
        switch (c->bal) {
            case LEFT: {
                a->bal = BAL;
                b->bal = RIGHT;
                break;
            } case RIGHT: {
                a->bal = LEFT;
                b->bal = BAL;
                break;
            } case BAL: {
                a->bal = b->bal = BAL;
                break;
            }
        }

        c->bal = BAL;
        a->right = rotateRight(b);
        a = rotateLeft(a);
    }

    return a;
}

AVLTree fixLeft (AVLTree a) {
    
    AVLTree b, c;
    b = a->left;

    if (b->bal == LEFT) {
        a->bal = b->bal = BAL;
        a = rotateRight(a);
    } else {
        c = b->right;
        switch (c->bal) {
            case RIGHT: {
                a->bal = BAL;
                b->bal = LEFT;
                break;
            } case LEFT: {
                a->bal = RIGHT;
                b->bal = BAL;
                break;
            } case BAL: {
                a->bal = b->bal = BAL; 
                break;
            } 
        }

        c->bal = BAL;
        a->left = rotateLeft(b);
        a = rotateRight(a);
    }

    return a;
}

// A árvore recebida está balanceada
// A árvore produzida está balanceada
AVLTree updateAVLRec(AVLTree a, int k, int i, int *g, int *u) {
    if (a == NULL) {
        a = malloc(sizeof(struct avl));
        a->key = k;
        a->info = i;
        a->bal = BAL;
        *g = 1; *u = 0;
    } else if (a->key == k) {
        a->info = i;
        *g = 0; *u = 1;
    } else if (a->key > k) {
        a->left = updateAVLRec(a->left, k, i, g, u);
        if (*g == 1)
            switch (a->bal) {
                case LEFT: {
                    a = fixLeft(a); *g = 0;
                    break;
                } case BAL: {
                    a->bal = LEFT;
                    break;
                } case RIGHT: {
                    a->bal = BAL; *g = 0;
                    break;
                }
            }
    } else {
        a->right = updateAVLRec(a->right, k, i, g, u);
        if (*g == 1)
            switch (a->bal) {
                case RIGHT: {
                    a = fixRight(a); *g = 0;
                    break;
                } case BAL: {
                    a->bal = RIGHT;
                    break;
                } case LEFT: {
                    a->bal = BAL; *g = 0;
                    break;
                }
            }
    }
    return a;
}

int updateAVL (AVLTree *a, int k, int i) {
    int g, u = 0;
    *a  = updateAVLRec (*a, k, i, &g, &u);
    return u;
}