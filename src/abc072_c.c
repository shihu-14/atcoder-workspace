#include <stdio.h>
#include <stdlib.h>

#define D 100010

const int INF = 1001001001;
int main(void)
{
    int n; scanf("%d", &n);
    int cnt[D];
    for (int i = 0; i < D; i++) cnt[i] = 0;
    for (int i = 0; i < n; i++)
    {
        int a; 
        scanf("%d", &a);
        a++;
        cnt[a]++;
        cnt[a-1]++;
        cnt[a+1]++;
    }
    int ans = 0;
    for (int i = 0; i < D; i++)
    {
        if (ans < cnt[i])
        {
            ans = cnt[i];
        }
    }
    printf("%d\n", ans);
    return 0;
}