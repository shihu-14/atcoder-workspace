#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n; scanf("%d", &n);
    int a[n];
    long long sum = 0;
    for (int i = 0; i < n; i++)
    {  
        scanf("%d", a+i);
        sum += a[i];
    }
    long long tmp_sum = 0, ans = (1ULL<<63)-1;
    for (int i = 0; i < n-1; i++)
    {
        tmp_sum += a[i];
        long long diff = llabs(tmp_sum-(sum-tmp_sum));
        if (diff < ans)
        {
            ans = diff;
        }
    }
    printf("%lld\n", ans);
    return 0;
}   