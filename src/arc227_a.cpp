#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define rep(i, n) for (int i = 0; i < (n); i++) 
using namespace std;

int main()
{
    int n; cin >> n;
    vector<string> s(3);
    rep(i, 3) cin >> s[i];
    vector<vector<int>> pos(3);
    
    rep(i, 3)
    {
        rep(j, 2*n) if (s[i][j] == '1') pos[i].emplace_back(j);
    }
    
    string ans(2*n, '0');
    // cout << ans << '\n';
    long long dist = 0;
    rep(i, n)
    {
        vector<int> tmp;
        rep(j, 3) tmp.emplace_back(pos[j][i]);
        sort(tmp.begin(), tmp.end());
        ans[tmp[1]] = '1';
        rep(j, 3) dist += abs(tmp[1]-tmp[j]);
    }

    cout << dist << '\n';
    cout << ans << '\n';
    return 0;
}