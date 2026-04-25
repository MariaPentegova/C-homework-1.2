#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000

typedef struct {
    int u;
    int v;
    int len;
} Edge;

void read_input(int *n, int *m, Edge **edges, int *k, int **capitals);
void process_borders(int n, int m, Edge *edges, int k, int *capitals);
void print_results(int n, int k, int *owner);

void solve(int n, int m, Edge *edges, int k, int *capitals) {
    int *owner = malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) {
        owner[i] = 0; //номер государства
    }

    for (int i = 0; i < k; i++) {
        owner[capitals[i]] = i + 1;
    }

    int assigned = k; //захвачено только k столиц пока что
    while (assigned < n) {
        for (int state = 1; state <= k; state++) {
            if (assigned >= n) {
                break;
            }

            int min_dist = INF;
            int best_city = -1;

            for (int i = 0; i < m; i++) {
                int u = edges[i].u;
                int v = edges[i].v;
                int len = edges[i].len;

                if (owner[u] == state && owner[v] == 0) {
                    if (len < min_dist) {
                        min_dist = len;
                        best_city = v;
                    }
                }
                if (owner[v] == state && owner[u] == 0) {
                    if (len < min_dist) {
                        min_dist = len;
                        best_city = u;
                    }
                }
            }
            if (best_city != -1) {
                owner[best_city] = state;
                assigned++;
            }
        }
    }

    print_results(n, k, owner);
    free(owner);
}

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
