#include <stdio.h>
#include <stdlib.h>
#include "nations.h"

#define INF 1000000000

void read_input(int *n, int *m, Edge **edges, int *k, int **capitals) {
    scanf("%d %d", n, m);
    *edges = malloc((*m) * sizeof(Edge));
    for (int i = 0; i < *m; i++) {
        scanf("%d %d %d", &(*edges)[i].u, &(*edges)[i].v, &(*edges)[i].len);
    }
    scanf("%d", k);
    *capitals = malloc((*k) * sizeof(int));
    for (int i = 0; i < *k; i++) {
        scanf("%d", &(*capitals)[i]);
    }
}

void print_results(int n, int k, int *owner) {
    for (int state = 1; state <= k; state++) {
        printf("Государство %d: ", state);
        for (int i = 1; i <= n; i++) {
            if (owner[i] == state) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }
}

int main() {
    int n, m, k;
    Edge *edges = NULL;
    int *capitals = NULL;
    
    read_input(&n, &m, &edges, &k, &capitals);

    solve(n, m, edges, k, capitals);

    free(edges);
    free(capitals);

    return 0;
}
