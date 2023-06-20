#ifndef CHAINING_H
#define CHAINING_H

#define Size 10
typedef struct _nodo Nodo, *THash [Size];

void print_ht       (THash t);
void initEmpty      (THash t);
void add            (char *s, THash t);
int  lookup         (char *s, THash t);
void remove_element (char *s, THash t);

#endif // CHAINING_H