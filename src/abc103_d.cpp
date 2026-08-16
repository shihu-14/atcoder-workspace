#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>

using namespace std;

int main()
{
    int n, m; cin >> n >> m;
    vector<vector<int> > ls(n);
    vector<pair<int, int>> query;
    for (int i = 0; i < m; i++)
    {
        int a, b; cin >> a >> b; a--, b--;
        query.emplace_back(a, b);
        ls[b].emplace_back(a);
    }
    sort(query.begin(), query.end());
    set<int> block;
    int ans = 0;
    for (int r = 0; r < n; r++)
    {
        if (ls[r].size() == 0) continue;
        int max_l = -1;
        for (int l: ls[r])
        {
            max_l = max(max_l, l);
        }
        while(block.size() && *block.begin() < max_l)
        {
            block.erase(block.begin());
        }
        if (block.size() == 0)
        {
            ans++;
            block.emplace(r-1);
        }
    }
    cout << ans << '\n';
    return 0;
}