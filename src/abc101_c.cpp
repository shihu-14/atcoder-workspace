#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int n, k; cin >> n >> k;
    // cout << (n-1+k-1-1)/(k-1) << '\n';
    // return 0;
    vector<int> a(n);
    int zero_id = 0;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i]; a[i]--;
        if (a[i] == 0) zero_id = i;
    }
    if (zero_id < k)
    {
        int ans = 0;
        int rem = n-1;
        if (zero_id != 0)
        {
            ans++;
            rem = n-k;
        }
        ans += (rem+k-2)/(k-1);
        cout << ans << '\n';
    }
    else if (zero_id >= n-k)
    {
        int ans = 0;
        int rem = n-1;
        if (zero_id != n-1)
        {
            ans++;
            rem = zero_id;
        }
        ans += (rem+k-2)/(k-1);
        cout << ans << '\n';
    }
    else
    {
        int ans = 0;
        int rem_l = zero_id;
        int q = (k+rem_l-2)/(k-1);
        ans += q;
        int rem = q*(k-1)-rem_l;
        int rem_r = n-zero_id-1-rem;
        ans += (k+rem_r-2)/(k-1);        
        cout << ans << '\n';
    }
    
    return 0;
}