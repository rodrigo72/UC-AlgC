#include "open_addressing.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// No caso de acontecer uma colisão é calculada uma nova posição para armazenar esse par
// Este processo de calcular uma posição alternativa é chamado de probing

// LINEAR PROBING
// Caso a posição esteja ocupada, tenta-se na posição seguinte

static int where (char *s, THash_2 t) {
    return hash(s) % Size;
}

static int find_linear_probe (char *s, THash_2 t) {
    int p = where(s, t);
    int count;
    
    for (count = Size; count > 0 && t[p].status != Free && strcmp(t[p].chave, s); count--) {
        p = (p+1) % Size;
    }

    if (count == 0) p = -1; // full
    return p;
} 

// Se a chave existir (apagada ou efetiva) a função retorna a posição correspontente
// Se a chave não existir a função retorna a primeira posição livre ou ocupada 
// a partir da posição dada pela função de hash

static int find_linear_probe_2 (char *s, THash_2 t) {
    int p = where(s, t);
    int count, tmp;

    // p aumenta no maximo até Size-1
    // p aumenta enquando as posiçoes percorridas estao usadas 
    // e a chave das posiçoes é diferente da key s que se procura
    for (count = Size; count > 0 && t[p].status == Used && strcmp(s, t[p].chave); count--) {
        p = (p+1) % Size;
    }

    // se a chave não existir
    if (t[p].chave == NULL || strcmp(s, t[p].chave))
        if (count == 0) {   // a tabela está cheia (todas as celulas Used), logo nao é possivel encontrar uma posição para a chave
            p = -1;
        } else {            // foi encontrada uma célula apagada
            tmp = p;
            // a posição p aumenta enquanto nao se encontrar uma celula livre ou uma chave igual
            while (count > 0 && t[p].status != Free && (t[p].chave == NULL || strcmp(s, t[p].chave))) {
                p = (p+1) % Size;
                count--;
            }
            // se o ciclo parou devido à chave ser igual ent devolve p senao devolve a posiçao da celula apagada
            if (t[p].chave == NULL || strcmp(s, t[p].chave)) p = tmp;
        }
    // else => a chave foi econtrada por isso retorna p

    return p;
}

// A estratégia Linear Probing tem como principal desvantagem não evitar clusters de células
// ocupadas, aumentado por isso o número de colisões secundarias. 
// Uma forma de evitar isso consiste em ir aumentando o espaçamento entre probes sucessivos.
// Assim, em vez de tentarmos na posição imediatamente seguinte, procuramos d posições à
// frente, incrementando esta medida d.

static int find_quadratic_probe (char *s, THash_2 t) {
    int p = where(s, t);
    int count, d = 1;

    for (count = Size; count > 0 && t[p].status != Free && strcmp(s, t[p].chave); count--) {
        p = (p+d) % Size;
        d++;
    }

    if (count == 0) 
        p = -1;

    return p;
}

static int find_quadratic_probe_2 (char *s, THash_2 t) {
    int p = where(s, t);
    int count, tmp, d = 1;

    for (count = Size; count > 0 && t[p].status == Used && strcmp(s, t[p].chave); count--) {
        p = (p+d) % Size;
        d++;
    }

    if (t[p].chave == NULL || strcmp(s, t[p].chave))
        if (count == 0) {   // full table
            p = -1;
        } else {            // deleted cell found
            tmp = p;
            while (count > 0 && t[p].status != Free && (t[p].chave == NULL || strcmp(s, t[p].chave))) {
                p = (p+d) % Size;
                count--;
                d++;
            }
            if (t[p].chave == NULL || strcmp(s, t[p].chave)) p = tmp;
        }
        
    return p;
}

// ----------------------------------------------------------

void print_ht_2 (THash_2 t) {
    printf("\n-----\nstart\n-----\n");
    for (int i = 0; i < Size; i++) {
        printf("%d ", i);
        if (t[i].status == Used) {
            printf("-> [ %s : %d ] ", t[i].chave, t[i].ocorr);
        } else if (t[i].status == Del) {
            printf("-> (Del) ");
        }
        printf("\n");
    }
    printf("-----\n end\n-----\n\n");
}

void initEmpty_L2 (THash_2 t) {
    for (int i = 0; i < Size; i++) {
        t[i].status = Free;
        t[i].chave = NULL;
        t[i].ocorr = 0;
    }
}

void add_L2 (char *s, THash_2 t) {

    int p;
    if (LP) {
        p = find_linear_probe_2(s, t);
    } else {
        p = find_quadratic_probe_2(s, t);
    }

    if (p < 0) return; // full

    if (t[p].chave != NULL && !strcmp(s, t[p].chave)) { // key exists
        t[p].status = Used;
        t[p].ocorr++;
    } else {                                            // new key
        t[p].status = Used;
        t[p].chave = s;
        t[p].ocorr = 1;
    }
}

int lookup_L2 (char *s, THash_2 t) {
    
    int p;
    if (LP) {
        p = find_linear_probe_2(s, t);
    } else {
        p = find_quadratic_probe_2(s, t);
    }

    if (p >= 0 && t[p].status == Used && !strcmp(s, t[p].chave)) {
        return t[p].ocorr;
    }

    return 0;
}

int remove_element_L2 (char *s, THash_2 t) {
    
    int p;
    if (LP) {
        p = find_linear_probe_2(s, t);
    } else {
        p = find_quadratic_probe_2(s, t);
    }

    if (p >= 0 && t[p].status == Used && !strcmp(s, t[p].chave)) { // key exists
        if (t[p].ocorr == 1) {
            t[p].status = Del;
            t[p].chave  = NULL;
            t[p].ocorr  = 0;
        } else {
            t[p].ocorr--;
        }
    }
}

// reconstroi a tabela t de forma a nao haverem chaves apagadas
int garbage_collection (THash_2 t) {

    char *keys[Size];
    int ocorr[Size];

    int j = 0;
    for (int i = 0; i < Size; i++) {
        if (t[i].status == Used) {
            keys[j] = t[i].chave;
            ocorr[j] = t[i].ocorr;
            j++;
        }
    }

    initEmpty_L2(t);

    for (int i = 0; i < j; i++) {
        t[i].status = Used;
        t[i].chave = keys[i];
        t[i].ocorr = ocorr[i];
    }

}