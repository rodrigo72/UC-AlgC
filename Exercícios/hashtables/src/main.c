#include "chaining.h"
#include "open_addressing.h"

#include <stdio.h>

int main(void) {
    
    printf("Testing: ");
    char c = getchar();

    if (c == '1') {

        THash t;
        initEmpty(t);

        add("asd", t);
        add("bom dia", t);

        print_ht(t);
        printf("Lookup 'asd' : %d\n", lookup("bom dia", t));
        remove_element("asd", t);
        printf("'asd' removed\n");
        print_ht(t);

    } else if (c == '2') {
        
        THash_2 t2;
        initEmpty_L2(t2);

        char s[]  = "yes";
        char s2[] = "hell";

        add_L2(s, t2);
        add_L2(s2, t2);
        add_L2("a", t2);
        add_L2("b", t2);
        add_L2("c", t2);
        add_L2("d", t2);
        add_L2("e", t2);
        add_L2("e", t2);
        add_L2("e", t2);
        add_L2("f", t2);
        add_L2("g", t2);
        add_L2("h", t2);

        print_ht_2(t2);

        printf("lookup 'yes': %d\n", lookup_L2(s, t2));
        remove_element_L2(s, t2);
        print_ht_2(t2);

        add_L2("i", t2);
        add_L2("i", t2);
        print_ht_2(t2);

        remove_element_L2(s2, t2);
        print_ht_2(t2);

        garbage_collection(t2);
        print_ht_2(t2);
    }

    return 0;
}