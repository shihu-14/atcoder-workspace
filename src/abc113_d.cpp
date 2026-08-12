#include <iostream>
#include <vector>
#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;
// using namespace std;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main()
{
    int h, w, k; cin >> h >> w >> k; k--;
    vector<mint> dp(w); dp[0] = 1;
    for (int i = 0; i < h; i++)
    {
        vector<mint> old(w); swap(old, dp);
        for (int j = 0; j < w; j++)
        {
            int n = w-1;
            int n2 = 1<<n;
            for (int s = 0; s < n2; s++)
            {
                bool flag = false;
                if (s&s>>1)
                {
                    continue;
                }
                // for (int x = 0; x < n-1; x++)
                // {
                //     if (s>>x&1 && s>>(x+1)&1)
                //     {
                //         flag = true;
                //         break;
                //     }
                // }
                if (flag) continue;
                // cout << i << " " << j << " " << s << endl;
                if (j >= 1 && s>>(j-1)&1)
                {
                    dp[j-1] += old[j];
                }
                else if (j+1 < w; s>>j&1)
                {
                    dp[j+1] += old[j];
                }
                else
                {
                    dp[j] += old[j];
                }
            }
        }
    }
    cout << dp[k].val() << endl;
    return 0;
}