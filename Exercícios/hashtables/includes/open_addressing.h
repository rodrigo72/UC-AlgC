#ifndef OPEN_ADDRESSING_H
#define OPEN_ADDRESSING_H

#define Size 10

#define Free 0
#define Used 1
#define Del 2

#define LP 1

typedef struct _bucket {
    int status;  // Free | Used | Del
    char *chave; 
    int ocorr;   // número de ocorrências
} THash_2 [Size];

void initEmpty_L2      (THash_2 t);
void add_L2            (char *s, THash_2 t);
int lookup_L2          (char *s, THash_2 t);
void print_ht_2        (THash_2 t);
int remove_element_L2  (char *s, THash_2 t);
int garbage_collection (THash_2 t);

#endif // OPEN_ADDRESSING_H