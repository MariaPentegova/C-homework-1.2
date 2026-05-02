#include <stdio.h>
#include <stdlib.h>
#include "nations.h"

int main() {
    int n, m; 
    scanf("%d %d", &n, &m);
    Edge *edges = malloc(m * sizeof(Edge));
    if (!edges) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
    }

    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].len);
    }

    int k; 
    scanf("%d", &k);
  
    int *capitals = malloc(k * sizeof(int));
    if (!capitals) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        free(edges);
        return 1;
    }
    for (int i = 0; i < k; i++) {
        scanf("%d", &capitals[i]);
    }
  
    return 0;
}

