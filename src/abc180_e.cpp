#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const int inf = 1001001001;
void chmin(int &a, int b){a = min(a, b);}
int main(){
    int n; cin >> n;
    vector<T> p;
    rep(i, n){
        int x, y, z; cin >> x >> y >> z;
        p.emplace_back(x, y, z);
    }
    int m = 1<<n;
    vector<vector<int>> dp(m, vector<int>(n, inf));
    rep(i, n){
        if (i == 0) continue;
        auto [p1, q1, r1] = p[i];
        auto [p2, q2, r2] = p[0];
        dp[1<<i][i] = abs(p1-p2)+abs(q1-q2)+max(0, r1-r2);
    }
    rep(i, m){
        rep(j, n){
            if (~i>>j&1) continue;
            rep(k, n){
                if (i>>k&1) continue;
                auto [p1, q1, r1] = p[j];
                auto [p2, q2, r2] = p[k];
                int cost = abs(p1-p2)+abs(q1-q2)+max(0, r2-r1);
                chmin(dp[i|(1<<k)][k], dp[i][j]+cost);
            }
        }
    }
    cout << dp[m-1][0] << endl;
    return 0;
}