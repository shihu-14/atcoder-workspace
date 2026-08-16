#include <iostream>
#include <vector>
#include <atcoder/modint>

using namespace atcoder;
using namespace std;
using mint = modint1000000007;

int main()
{
    string s; cin >> s;
    int n = s.size();
    vector<mint> dp(4); dp[0] = 1;
    mint ans;
    for (int i = 0; i < n; i++)
    {
        vector<mint> old(4); swap(old, dp);
        for (int j = 0; j < 4; j++)
        {
            if (s[i] == '?')
            {
                dp[j] += 3*old[j];
                for (int c = 0; c < 3; c++)
                {
                    if (j == c)
                    {
                        dp[j+1] += old[j];
                    }
                }
            }
            else
            {
                dp[j] += old[j];
                int c = s[i]-'A';
                if (j == c)
                {
                    dp[j+1] += old[j];
                }
            }
        }
        for (int j = 0; j < 4; j++)
        {
            cout << dp[j].val() << " ";
        }
        cout << endl;
    }
    cout << dp[3].val() << '\n';
    return 0;
}