#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <atcoder/modint>

using namespace std;
using namespace atcoder;
using mint = modint1000000007;

int main()
{
    int n, m; cin >> n >> m;
    if (m == 1)
    {
        cout << 1 << '\n';
        return 0;
    }
    vector<int> ds;
    vector<mint> fact(n+1, 1), fact_inv(n+1);
    for (int i = 0; i < n; i++) fact[i+1] = fact[i]*(i+1);
    fact_inv[n] = fact[n].inv();
    for (int i = n-1; i >= 0; i--) fact_inv[i] = fact_inv[i+1]*(i+1);
    for (int i = 1; (long long)i*i <= m; i++)
    {
        if (m%i) continue;
        ds.emplace_back(i);
    }
    sort(ds.begin(), ds.end());

    mint ans = 0;
    vector<int> stk;
    auto f = [&](auto f, int id, int val) -> void
    {
        if (val == 1)
        {
            mint res = fact[n];
            for (int i = 0; i < stk.size();)
            {
                int j = i;
                while(j < stk.size() && stk[i] == stk[j])
                {
                    j++;
                }
                res *= fact_inv[j-i];
                i = j;
            }
            res *= fact_inv[n-stk.size()];
            ans += res;
            return;
        }
        for (int i = id; i < ds.size(); i++)
        {
            int d = ds[i];
            // cout << d << " " << val << endl;
            if (val < (long long)d*d) break;
            if (val%d) continue;
            if (stk.size() < n)
            {
                // cout << d << " " << val << "-> " << nval << endl;
                stk.emplace_back(d);
                f(f, i, val/d);
                stk.pop_back();
            }
        }
        if (val >= ds[id])
        {
            if (stk.size() < n)
            {
                // cout << d << " " << val << "-> " << nval << endl;
                stk.emplace_back(val);
                f(f, ds.size(), 1);
                stk.pop_back();
            }
        }
    };
    f(f, 1, m);
    cout << ans.val() << '\n';
    return 0;
}