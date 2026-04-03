#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXN 10000
#define MAXM 20000

typedef struct Road {
    int to;
    int len;
    struct Road* next;
} Road;

Road* graph[MAXN];

void add_road(int u, int v, int len) {
    Road* r1 = (Road*)malloc(sizeof(Road));
    r1->to = v;
    r1->len = len;
    r1->next = graph[u];
    graph[u] = r1;

    Road* r2 = (Road*)malloc(sizeof(Road));
    r2->to = u;
    r2->len = len;
    r2->next = graph[v];
    graph[v] = r2;
}

typedef struct {
    int dist;
    int city;
    int state;
} Node;

Node heap[MAXM];
int heap_size = 0;

void swap(Node* a, Node* b) {
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

void push(Node x) {
    heap[++heap_size] = x;
    int i = heap_size;
    while (i > 1 && heap[i].dist < heap[i / 2].dist) {
        swap(&heap[i], &heap[i / 2]);
        i /= 2;
    }
}

Node pop() {
    Node res = heap[1];
    heap[1] = heap[heap_size--];

    int i = 1;
    while (1) {
        int left = 2 * i, right = 2 * i + 1, smallest = i;

        if (left <= heap_size && heap[left].dist < heap[smallest].dist)
            smallest = left;
        if (right <= heap_size && heap[right].dist < heap[smallest].dist)
            smallest = right;

        if (smallest == i) break;

        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }

    return res;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    for (int i = 1; i <= n; i++)
        graph[i] = NULL;

    for (int i = 0; i < m; i++) {
        int u, v, len;
        scanf("%d %d %d", &u, &v, &len);
        add_edge(u, v, len);
    }

    int k;
    scanf("%d", &k);

    int capitals[k];
    for (int i = 0; i < k; i++)
        scanf("%d", &capitals[i]);

    int dist[MAXN];
    int state[MAXN];

    for (int i = 1; i <= n; i++) {
        dist[i] = INT_MAX;
        state[i] = -1;
    }

    // Инициализация столиц
    for (int i = 0; i < k; i++) {
        int c = capitals[i];
        dist[c] = 0;
        state[c] = i;
        push((Node){0, c, i});
    }

    // Мульти-Дейкстра
    while (heap_size > 0) {
        Node cur = pop();

        if (cur.dist > dist[cur.city]) continue;

        Road* r = graph[cur.city];
        while (r) {
            int to = r->to;
            int nd = cur.dist + r->len;

            if (nd < dist[to]) {
                dist[to] = nd;
                state[to] = cur.state;
                push((Node){nd, to, cur.state});
            }

            r = r->next;
        }
    }

    for (int i = 0; i < k; i++) {
        printf("State %d:", i);
        for (int j = 1; j <= n; j++) {
            if (state[j] == i)
                printf(" %d", j);
        }
        printf("\n");
    }

    return 0;
}
