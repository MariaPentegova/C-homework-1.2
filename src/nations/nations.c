#include <stdio.h>
#include <stdlib.h>
#include "nations.h"

#define INF 1000000000

static void assign_initial_capitals(int n, int k, int *capitals, int *owner) {
    for (int i = 1; i <= n; i++) {
        owner[i] = 0;
    }
    for (int i = 0; i < k; i++) {
        owner[capitals[i]] = i + 1;
    }
}

static int find_closest_city(int owner_id, int m, Edge *edges, int *owner) {
    int min_len = INF;
    int best_city = -1;
    for (int i = 0; i < m; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int len = edges[i].len;

        if (owner[u] == owner_id && owner[v] == 0 && len < min_len) {
            min_len = len;
            best_city = v;
        }
        if (owner[v] == owner_id && owner[u] == 0 && len < min_len) {
            min_len = len;
            best_city = u;
        }
    }
    return best_city;
}

static void expand_territories(int n, int m, Edge *edges, int k, int *owner) {
    int assigned = k;
    while (assigned < n) {
        int prev_assigned = assigned;
        for (int state = 1; state <= k; state++) {
            if (assigned >= n) {
                break;
            }
            int city_to_add = find_closest_city(state, m, edges, owner);
            if (city_to_add != -1 && owner[city_to_add] == 0) {
                owner[city_to_add] = state;
                assigned++;
            }
        }
        if (assigned == prev_assigned) {
            break;
        }
    }
}

static void print_results(int n, int k, int *owner) {
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

void solve(int n, int m, Edge *edges, int k, int *capitals) {
    int *owner = malloc((n + 1) * sizeof(int));
    if (!owner) {
        fprintf(stderr, "Failed to allocate memory for owner array\n");
        return;
    }

    assign_initial_capitals(n, k, capitals, owner);
    expand_territories(n, m, edges, k, owner);
    print_results(n, k, owner);

    free(owner);
}
