#ifndef MIN_HEAP_H_
#define MIN_HEAP_H_

typedef struct bst {
    int key, info;
    struct bst *left, *right;
} *BST;

typedef struct avl {
    int bal;
    int key, info;
    struct avl *left, *right;
} *AVLTree;

void print_t (AVLTree tree);
void print_bst (BST a, int space);
int update (BST *a, int k, int i);
int updateAVL (AVLTree *a, int k, int i);
int lookup (BST a, int k, int *i);

#endif // MIN_HEAP_H_