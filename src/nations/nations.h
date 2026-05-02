#ifndef NATIONS_H
#define NATIONS_H

typedef struct {
    int u;
    int v;
    int len;
} Edge;

void solve(int n, int m, Edge *edges, int k, int *capitals);

#endif
