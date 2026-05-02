#ifndef Nations_H
#define Nations_H

typedef struct {
    int u;
    int v;
    int len;
} Edge;

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

#endif
