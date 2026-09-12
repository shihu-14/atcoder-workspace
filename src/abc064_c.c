#include <stdio.h>

int main(void)
{
    int n; scanf("%d", &n);
    int cnt[9] = {0}; 
    for (int i = 0; i < n; i++)
    {
        int a; scanf("%d", &a);
        int q = a/400;
        if (q >= 9) q = 8;
        cnt[q]++;
    }
    int ans = 0;
    for (int i = 0; i <= 7; i++)
    {
        if (cnt[i] > 0) ans++;
    }
    if (ans == 0)
    {
        printf("%d %d\n", cnt[8] == 0 ? 0: 1, cnt[8]);
    }
    else
    {
        printf("%d %d\n", ans, ans+cnt[8]);
    }
    return 0;
}