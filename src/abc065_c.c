#include <stdio.h>
#include <stdlib.h>

#define MOD 1000000007
#define D 100010
int main(void)
{
    int n, m;
    scanf("%d %d", &n, &m);
    int d = abs(n-m);
    if (d > 1)
    {
        printf("%d\n", 0);
        return 0;
    }
    int fact[D] = {1};
    for (int i = 0; i < D-1; i++)
    {
        fact[i+1] = (long long)fact[i]*(i+1)%MOD;
    }
    int ans = (long long)fact[n]*fact[m]%MOD;
    if (d == 0) ans = ans*2%MOD;
    printf("%d\n", ans);

    return 0;
}