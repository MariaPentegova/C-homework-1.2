#ifndef NATIONS_H
#define NATIONS_H

typedef struct {
    int u;
    int v;
    int len;
} Edge;

void solve(int n, int m, Edge *edges, int k, int *capitals);
static void assign_initial_capitals(int n, int k, int *capitals, int *owner);
static void expand_territories(int n, int m, Edge *edges, int k, int *owner);
static int find_closest_city(int owner_id, int m, Edge *edges, int *owner);
static void print_results(int n, int k, int *owner);

#endif // NATIONS_H
