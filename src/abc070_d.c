#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int to, cost;
} Edge;

Edge* g[100010];
int sz[100010];
long long dist[100010];

void dfs(int v, int p)
{
    for (int i = 0; i < sz[v]; i++)
    {
        Edge e = g[v][i];
        if (e.to == p) continue;
        dist[e.to] = dist[v]+e.cost;
        dfs(e.to, v);
    }
}
int main(void)
{
    int n; scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        sz[i] = 0;
        dist[i] = 0;
    }
    for (int i = 0; i < n-1; i++)
    {
        int a, b, c; 
        scanf("%d %d %d", &a, &b, &c);
        a--, b--;
        g[a] = realloc(g[a], sizeof(Edge)*(sz[a]+1));
        g[a][sz[a]++] = (Edge){b, c};
        g[b] = realloc(g[b], sizeof(Edge)*(sz[b]+1));
        g[b][sz[b]++] = (Edge){a, c};
    }
    int q, k; scanf("%d %d", &q, &k);
    k--;
    dfs(k, -1);
    for (int qi = 0; qi < q; qi++)
    {
        int x, y; scanf("%d %d", &x, &y);
        x--, y--;
        printf("%lld\n", dist[x]+dist[y]);
    }
    return 0;
}