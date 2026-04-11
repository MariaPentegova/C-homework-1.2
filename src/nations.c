#include <stdio.h>
#include <stdlib.h>

#define INF 1e9

typedef struct {
    int u, v, len;
} Edge;

int main() {
    int n, m, k;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    Edge *edges = malloc(m * sizeof(Edge));
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].len);
    }

    scanf("%d", &k);
    int *capitals = malloc(k * sizeof(int));
    int *owner = malloc((n + 1) * sizeof(int)); // 0 - свободен, 1..k - номер гос-ва

    for (int i = 1; i <= n; i++) owner[i] = 0;

    for (int i = 0; i < k; i++) {
        scanf("%d", &capitals[i]);
        owner[capitals[i]] = i + 1;
    }

    int assigned = k;
    while (assigned < n) {
        // Проходим по очереди по каждому государству
        for (int state = 1; state <= k; state++) {
            if (assigned >= n) break;

            int min_dist = INF;
            int best_city = -1;

            // Ищем ближайший город к текущему государству 'state'
            for (int i = 0; i < m; i++) {
                int u = edges[i].u;
                int v = edges[i].v;
                int len = edges[i].len;

                // Если u в государстве, а v свободен
                if (owner[u] == state && owner[v] == 0) {
                    if (len < min_dist) { min_dist = len; best_city = v; }
                }
                // Если v в государстве, а u свободен
                if (owner[v] == state && owner[u] == 0) {
                    if (len < min_dist) { min_dist = len; best_city = u; }
                }
            }

            // Если нашли город, забираем его
            if (best_city != -1) {
                owner[best_city] = state;
                assigned++;
            }
        }
    }

    for (int state = 1; state <= k; state++) {
        printf("Государство %d: ", state);
        for (int i = 1; i <= n; i++) {
            if (owner[i] == state) printf("%d ", i);
        }
        printf("\n");
    }

    free(edges); free(capitals); free(owner);
    return 0;
}
