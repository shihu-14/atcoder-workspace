#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int n, m, q; cin >> n >> m >> q;
    vector<int> r_cnt(n+1);
    vector<vector<int> > rs(n);
    for (int i = 0; i < m; i++)
    {
        int l, r; cin >> l >> r; l--;
        r_cnt[r]++;
        rs[l].emplace_back(r);
    }

    vector<vector<vector<int> >> query(n, vector<vector<int>>(n+1));
    for (int qi = 0; qi < q; qi++)
    {
        int p, q; cin >> p >> q; p--;
        query[p][q].emplace_back(qi);
    }
    vector<int> ans(q);
    for (int l = 0; l < n; l++)
    {
        int sum = 0;
        for (int r = l+1; r <= n; r++)
        {
            sum += r_cnt[r];
            for (auto id: query[l][r])
            {
                ans[id] = sum;
            }
        }
        for (auto r: rs[l])
        {
            r_cnt[r]--;
        }
    }
    for (int i = 0; i < q; i++)
    {
        cout << ans[i] << '\n';
    }
}