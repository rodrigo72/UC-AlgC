#include "utils.h"

unsigned hash (char *str) {
    unsigned hash = 5381;
    int c;

    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash;
}