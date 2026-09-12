#include <stdio.h>

typedef long long ll;
int main(void)
{
    ll K; scanf("%lld", &K);
    for (int n = 2; n <= 50; n++)
    {
        ll a[n];
        ll q = K/n, r = K%n;
        int flag = 1;
        for (int i = 0; i < n; i++)
        {
            ll v = n-1+(n+1)*q-K;
            if (i < r)
            {
                v += n+1;
            }
            if (v > 10000000000000000LL+1000)
            {
                flag = 0;
                break;
            }
            a[i] = v;
        }
        if (flag == 1)
        {
            printf("%d\n", n);
            for (int i = 0; i < n; i++)
            {
                printf("%lld ", a[i]);
            }
            printf("\n");
            return 0;
        }
    }
    return 0;
}