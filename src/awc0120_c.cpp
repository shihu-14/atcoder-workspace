#include <iostream>
#include <vector>

using namespace std;
using ll = long long;
int main()
{
    int n; cin >> n;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        int p; cin >> p; p--;
        g[p].emplace_back(i);
    }
    vector<ll> V(n);
    for (int i = 0; i < n; i++)
    {
        cin >> V[i];
    }
    auto dfs = [&](auto f, int v) -> bool
    {
        ll sum = 0;
        for (int u: g[v])
        {
            if (f(f, u)) return true;
            sum += V[u];
        }
        if (sum > V[v])
        {
            return true;
        }
        return false;
    };
    if (dfs(dfs, 0))
    {
        cout << "No" << endl;
    }
    else
    {
        cout << "Yes" << endl;
    }
}