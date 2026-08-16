#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int n; cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; i++)
    {
        cin >> a[i];
        a[i] -= i+1;
    }
    sort(a.begin(), a.end());
    int m = a[n/2];
    if (n%2 == 0)
    {
        m = (a[n/2]+a[n/2-1])/2;
    }
    long long ans = 0;
    for (int i = 0; i < n; i++)
    {
        ans += abs(a[i]-m);
    }
    cout << ans << '\n';
    return 0;
}
