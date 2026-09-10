#include <stdio.h>

int main(void)
{
    int n, m; scanf("%d %d", &n, &m);
    int from1[n], toN[n];
     for (int i = 0; i < n; i++)
    {
        from1[i] = 0;
        toN[i] = 0;
    }
    for (int i = 0; i < m; i++)
    {
        int a, b; scanf("%d %d", &a, &b);
        a--, b--;
        if (a == 0)
        {
            from1[b] = 1;
        }
        else if (b == n-1)
        {
            toN[a] = 1;
        }
    }
    for (int i = 1; i < n-1; i++)
    {
        if (from1[i]+toN[i] == 2)
        {
            printf("POSSIBLE\n");
            return 0;
        }
    }
    printf("IMPOSSIBLE\n");
    return 0;
}