#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void chmin(int &a, int b){a = min(a, b);}
const int INF = 1001002003;
int main()
{   
    int n; cin >> n;
    vector<int> memo(n+1, -1);
    auto f = [&](auto f, int n) -> int
    {
        // cout << n << endl;
        if (n == 0) return 0;
        if (memo[n] != -1) return memo[n];
        int res = INF;
        { // six
            int a = 1;
            for (int i = 1; i <= n; i *= 6)
            {
                chmin(res, f(f, n-i)+1);
            }
        }
        { // nine
            int a = 1;
            for (int i = 1; i <= n; i *= 9)
            {
                chmin(res, f(f, n-i)+1);
            }
        }
        return memo[n] = res;
    };
    int ans = f(f, n);
    // for (int i = 0; i <= n; i++)
    // {
    //     if (memo[i] != -1)
    //     cout << i << " " << memo[i] << '\n';
    // }
    cout << ans << '\n';
    return 0;   
}