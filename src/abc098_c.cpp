#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int n; cin >> n;
    string s; cin >> s;
    vector<int> sl(n+1), sr(n+1);
    for (int i = 0; i < n; i++)
    {
        sl[i+1] = sl[i]+(s[i]=='W');
        sr[i+1] = sr[i]+(s[i]=='E');
    }
    int ans = n;
    for (int i = 0; i < n; i++)
    {
        // cout << sr[i]+sl[n]-sl[i+1] << '\n';
        ans = min(ans, sl[i]+sr[n]-sr[i+1]);
    }
    cout << ans << '\n';
}