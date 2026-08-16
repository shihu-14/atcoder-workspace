#include <iostream>
#include <vector>
using namesapce std;

int main()
{
    int n, k; cin >> n >> k;
    vector<int> a(n);
    int zero_id = 0;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i]; a[i]--;
        if (a[i] == 0) zero_id = i;
    }
    if (zero_id < k)
    {
        int rem = n-k;
        cout << (k+rem)/rem+1 << '\n';
    }
    else
    {
        int rem_l = k, rem_r = n-k-1;
        cout << (k+rem_l)/2+(k+rem_r)/2 << '\n';
    }
    return 0;
}