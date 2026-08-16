#include <iostream>
#include <vector>
#include <array>

using namespace std;
using ll = long long;


void chmax(ll &a, ll b){a = max(a, b);}
const ll INF = 3001002003004005006;
const int D = 3;


int main()
{
    int n, m; cin >> n >> m;
    vector<vector<ll> > dp(m+1, vector<ll>(1<<D, -INF));
    for (int i = 0; i < 1<<D; i++) dp[0][i] = 0;
    for (int i = 0; i < n; i++)
    {
        vector<vector<ll> > old(m+1, vector<ll>(1<<D, -INF)); swap(old, dp);
        array<ll, D> a;
        for (int j = 0; j < D; j++)
        {
            cin >> a[j];
        }
        for (int j = 0; j <= m; j++)
        {
            for (int k = 0; k < 1<<D; k++)
            {
                if (old[j][k] == -INF) continue; 
                // not add
                chmax(dp[j][k], old[j][k]);
                // add
                ll add = 0;
                for (int l = 0; l < D; l++)
                {
                    if (k>>l&1) add += -a[l];
                    else add += a[l];
                }
                if (j+1 <= m)
                {
                    chmax(dp[j+1][k], old[j][k]+add);
                }
            }
        }
    }

    ll ans = -INF;
    for (int k = 0; k < 8; k++)
    {
        chmax(ans, dp[m][k]);
    }
    cout << ans << '\n';
    return 0;
}