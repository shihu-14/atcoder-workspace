#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n; cin >> n;
    vector<int> a, p;
    vector<bool> used(n+1);
    for (int i = 2; i <= n; i++)
    {
        if (used[i]) continue;
        p.emplace_back(i);
        for (int j = i; j <= n; j += i)
        {
            used[j] = true;
        }
        int cnt = 0;
        for (int j = i; j <= n; j *= i)
        {
            cnt += n/j;
        }
        a.emplace_back(cnt);
        // cout << i << "-> " << cnt << endl;
    }
    int ans = 0;
    // 3*25
    int m = p.size();
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i == j) continue;
            if (a[i] >= 2 && a[j] >= 24)
            {
                ans++;
            }
        }
    }
    // 5*15
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i == j) continue;
            if (a[i] >= 4 && a[j] >= 14)
            {
                ans++;
            }
        }

    }
    // 3*5*5
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            for (int k = 0; k < m; k++)
            {
                if (i == j || j >= k || k == i) continue;
                // cout << p[i] << " " << p[j] << " " << p[k] << endl;
                if (a[i] >= 2 && a[j] >= 4 && a[k] >= 4)
                {
                    ans++;
                }
            }
        }
    }
    // 75
    for (int i = 0; i < m; i++)
    {
        if (a[i] >= 74) ans++;
    }
    cout << ans << endl;
    return 0;
}