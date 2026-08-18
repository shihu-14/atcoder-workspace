#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    long long ans = 0;
    int sum = 0, r = 0;
    for (int l = 0; l < n; l++)
    {
        while(r < n && (sum&a[r]) == 0)
        {
            // cout << l << " " << r << " " << sum << "vs" << (sum^a[r]) << '\n';
            sum ^= a[r];
            r++;
        }
        // cout << l << " " << r << " " << sum << '\n';
        ans += r-l;
        sum ^= a[l];
    }
    cout << ans << '\n';
    return 0;
}