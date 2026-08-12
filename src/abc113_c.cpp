#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;

int main()
{
    int n, m; cin >> n >> m;
    vector<vector<pair<int, int>>> pos(n);

    for (int i = 0; i < m; i++)
    {
        int p, y; cin >> p >> y; p--;
        pos[p].emplace_back(y, i);
    }
    for (int i = 0; i < n; i++)
    {
        sort(pos[i].begin(), pos[i].end());
    }
    vector<pair<int, int>> ans(m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < pos[i].size(); j++)
        {
            ans[pos[i][j].second] = {i+1, j+1};
        }
    }
    for (int i = 0; i < m; i++)
    {
        auto [p, x] = ans[i];
        printf("%06d%06d\n", p, x);
    }
    return 0;
}