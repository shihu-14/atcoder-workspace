#include <stdio.h>

#define MOD 1000000007
typedef long long ll;
int main(void)
{
    int n; scanf("%d", &n);
    char s1[n+1], s2[n+1];
    scanf("%s", s1);
    scanf("%s", s2);

    ll a[3][3] = {0};
    ll b[3][3] = {0};
    ll (*dp)[3] = a;
    ll (*old)[3] = b;

    if (s1[0] == s2[0])
    {
        for (int i = 0; i < 3; i++)
        {
            dp[i][i] = 1;
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (i == j) continue;
                dp[i][j] = 1;
            }
        }
    }
    for (int i = 1; i < n; i++)
    {
        if (s1[i] == s1[i-1]) continue;
        ll (*tmp)[3] = dp;
        dp = old;
        old = tmp;
        for(int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                dp[j][k] = 0;
            }
        }
        if (s1[i] == s2[i])
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    for (int nj = 0; nj < 3; nj++)
                    {
                        if (nj == j || nj == k) continue;
                        dp[nj][nj] += old[j][k];
                        dp[nj][nj] %= MOD;
                    }
                }
            }
        }
        else
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    for (int nj = 0; nj < 3; nj++)
                    {
                        for (int nk = 0; nk < 3; nk++)
                        {
                            if (nj == j || nk == k || nj == nk) continue;
                            dp[nj][nk] += old[j][k];
                            dp[nj][nk] %= MOD;
                        }
                    }
                }
            }
        }
    }
    ll ans = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            ans += dp[i][j];
            ans %= MOD;
        }
    }
    printf("%lld\n", ans);
    return 0;
}