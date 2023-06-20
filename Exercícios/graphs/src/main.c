#include "adjacency_list.h"
#include "map.h"
#include "caminho.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>


int main() {

    GrafoL g1, g3, g5, g7;
    GrafoM g2 = {{0, 5, 0, 2, 0, 0, 0, 0, 0, 0} // 0
                ,{0, 0, 0, 0, 8, 0, 0, 0, 0, 0} // 1
                ,{0, 0, 0, 0, 0, 2, 0, 0, 0, 2} // 2
                ,{0, 0, 0, 0, 0, 0, 2, 0, 0, 0} // 3
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 4
                ,{0, 0, 0, 0, 0, 0, 0, 0, 8, 0} // 5
                ,{0, 0, 0, 0, 0, 0, 0, 1, 0, 0} // 6
                ,{0, 0, 0, 2, 7, 0, 0, 0, 0, 0} // 7
                ,{0, 0, 0, 0, 3, 0, 0, 0, 0, 4} // 8
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 9
                };
    
    GrafoM g4 = {{0, 0, 1, 0, 0, 1, 0, 0, 0, 0} // 0
                ,{0, 0, 0, 0, 0, 0, 0, 0, 1, 0} // 1
                ,{0, 0, 0, 1, 0, 0, 0, 0, 0, 0} // 2
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 3
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 1} // 4
                ,{0, 0, 0, 1, 0, 0, 1, 0, 0, 0} // 5
                ,{0, 0, 0, 1, 0, 0, 0, 0, 0, 0} // 6
                ,{0, 0, 0, 1, 0, 1, 0, 0, 0, 0} // 7
                ,{0, 0, 0, 0, 1, 0, 0, 1, 0, 4} // 8
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // 9
                };

    GrafoM g6 = {{0, 1, 0, 1, 0, 0, 0, 0, 0, 0}
                ,{1, 0, 1, 0, 0, 0, 0, 0, 0, 0}
                ,{0, 1, 0, 1, 0, 0, 0, 0, 0, 0}
                ,{1, 0, 1, 0, 0, 0, 0, 0, 0, 0}
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                ,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                };

    int max = 10;
    int cor[NV], f[NV] = {1};
    for (int i = 0; i < NV; i++) {
        cor[i] = rand() % max;
        f[i] += cor[i];
    }
    
    fromMat (g2, g1);
    fromMat (g4, g3);
    fromMat (g6, g5);
    fromMat (g6, g5);
    // print_adjacency_list (g3);
    // printf("\n");
    // inverte(g1, g1_2);
    // printf("%d", adj(g1, 4, 3));
    // print_adjacency_list (g1_2);
    // printf("%d\n", in_degree(g1));
    // printf("%d\n", colorOK(g1, cor));
    // printf("%d\n", homomorfOK(g1, g3, f));
    // topological_order (g1, 0);
    // printf("%d\n", biPartite(g3));

    int start = 2;
    int p[NV], c[NV];
    printf("\n%d // %d\n", maisLonga (g1, start, p), maisLonga_2(g1, start, p));

    directed_to_undirected(g2);

    // printf("\n  j\ni ");
    // for (int i = 0; i < NV; i++) {
    //     for (int j = 0; j < NV; j++)
    //         printf("%d ", g2[i][j]);
    //     printf("\n  ");
    // }
    // printf("\n\n");

    fromMat(g2, g7);
    // print_adjacency_list(g7);
    // print_adjacency_list(g7);
    printf("%d\n", componentes(g7, c));


    char *mapa_1[10] = {"##########"
                       ,"# #   #  #"
                       ,"# # # #  #"
                       ,"#   # #  #"
                       ,"##### #  #"
                       ,"#     #  #"
                       ,"## ####  #"
                       ,"#  #     #"
                       ,"#     #  #"
                       ,"##########"};

    Grafo g;
    map_to_graph(mapa_1, g, 10, 10);
    // print_graph(g);
    dijkskra_shortest_path(g, 2);
    int x = caminho (10, 10, mapa_1, 1, 1, 1, 8);
    printf("\n%d\n", x);

    int bombs[][5] = {{1,2,3},{2,3,1},{3,4,2},{4,5,3},{5,6,4}};
    int r = maximumDetonation(bombs, 5);
    printf("%d\n", r);

    return 0;
}