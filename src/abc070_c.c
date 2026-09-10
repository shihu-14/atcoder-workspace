#include <stdio.h>

typedef long long ll;
ll gcd(ll a, ll b)
{
    if (b == 0) return a;
    return gcd(b, a%b);
}
int main(void)
{
    int n; scanf("%d", &n);
    ll a[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%lld", a+i);
    }
    if (n == 1)
    {
        printf("%lld\n", a[0]);
        return 0;
    }
    ll lcm = a[0]/gcd(a[0], a[1])*a[1];
    for (int i = 1; i < n; i++)
    {
        lcm = a[i]/gcd(a[i], lcm)*lcm;
    }
    printf("%lld\n", lcm);
    return 0;
}