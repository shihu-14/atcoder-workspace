#include <iostream>
#include <vector>

using namespace std;
const long long INF = 3001002003004005006ll;

int main()
{
    int n, K, D; cin >> n >> K >> D; D--;
    vector<long long> a(n+1);
    for (int i = 0; i < n; i++) cin >> a[i+1];
    for (int i = 0; i < n; i++) a[i+1] += a[i]; 
    long long ans = -INF;
    for (int i = 0; i < n-K+1; i++)
    {
        if (i <= D && D < i+K)
        {
            ans = max(ans, a[i+K]-a[i]);
        }
    }
    cout << ans << endl;
}