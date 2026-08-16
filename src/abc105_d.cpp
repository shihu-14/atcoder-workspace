#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main()
{
    int n, m; cin >> n >> m;
    vector<int> s(n+1);
    for (int i = 0; i < n; i++)
    {
        cin >> s[i+1];
        s[i+1] %= m;
    }
    map<int, int> cnt;
    for (int i = 0; i < n; i++)
    {
        s[i+1] = (s[i+1]+s[i])%m;
        cnt[s[i+1]]++;
    }
    long long ans = 0;
    for (int l = 0; l < n; l++)
    {
        ans += cnt[s[l]];
        cnt[s[l+1]]--;
    }
    cout << ans << '\n';
    return 0;
}