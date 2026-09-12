#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define D 1000010

int *g[D];
int deg[D] = {0};
int dp[D] = {0};
int stk[D];
int path[D];
int top = 0;

bool dfs(int v, int p, int gv, bool flag)
{
    if (!flag && v == gv)
    {
        flag = true;
    }
    // printf("%d %d %d %d\n", v, p, top, flag);
    dp[v] = 1;
    for (int i = 0; i < deg[v]; i++)
    {
        int u = g[v][i];
        if (u == p) continue;
        if (!flag) stk[top++] = u;
        if (dfs(u, v, gv, flag))
        {
            flag = true;
        }
        if (!flag) top--;
        dp[v] += dp[u];
    }
    return flag;
}
int main(void)
{
    int n; scanf("%d", &n);
    for (int i = 0; i < n-1; i++)
    {
        int a, b; scanf("%d %d", &a, &b);
        a--, b--;
        g[a] = realloc(g[a], sizeof(int)*(deg[a]+1));
        g[a][deg[a]++] = b;
        g[b] = realloc(g[b], sizeof(int)*(deg[b]+1));
        g[b][deg[b]++] = a;
    }
    stk[top++] = 0;
    assert(dfs(0, -1, n-1, false));
    // printf("top:%d\n", top);
    // for (int i = 0; i < top; i++)
    // {
    //     printf("%d, ", stk[i]);
    // }
    // printf("\n");
    int sum1 = 0, sum2 = dp[stk[top-1]];
    for (int i = 0; i < top-1; i++)
    {
        int res = dp[stk[i]]-dp[stk[i+1]];
        if (i <= (top-1)/2)
        {
            sum1 += res;
        }
        else
        {
            sum2 += res;
        }
    }
    // printf("%d, %d\n", sum1, sum2);
    printf("%s\n", sum1 > sum2 ? "Fennec": "Snuke");
    return 0;
}