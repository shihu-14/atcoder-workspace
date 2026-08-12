#include <iostream>
#include <numeric>

using namespace std;
using ll = long long;
int main()
{
    int n, k; cin >> n >> k;
    int g = gcd(k, 2);
    ll ans = 0; 
    for (int a = k/g; a <= n; a+=k/g)
    {
        ll q = max(0, (n+a)/k-(a+k)/k+1);
        ans += q*q;
    }
    cout << ans << '\n';
    return 0;
}