#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

int main(){
    int n; cin >> n;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    vector dp(n+1, vector<int>(200, 0)); dp[0][0] = 1;
    rep(i, n){
        rep(j, 200){
            dp[i+1][j] += dp[i][j];
            dp[i+1][(j+a[i])%200] += dp[i][j];
            if (dp[i+1][j] > 3) dp[i+1][j] = 3;
            if (dp[i+1][(j+a[i])%200] > 3) dp[i+1][(j+a[i])%200] = 3;
        }
    }

    int r = -1;
    dp[n][0]--;
    rep(i, 200) if (dp[n][i] >= 2){
        r = i;
        break;
    }
    if (r == -1){
        cout << "No" << endl;
        return 0;
    }

    cout << "Yes" << endl;
    vector<int> tmp;
    int cnt = 0;
    auto f = [&](auto f, int i, int r) -> bool{
        if (i == -1){
            if (tmp.size() == 0) return false;
            cnt++;
            vector<int> tmp2 = tmp;
            sort(tmp2.begin(), tmp2.end());
            cout << tmp2.size() << " ";
            rep(i, tmp2.size()) cout << tmp2[i]+1 << " ";
            cout << endl;
            return (cnt >= 2);
        }
        int nr = (r - a[i])%200;
        if (nr < 0) nr += 200;
        if (dp[i][nr]){
            tmp.push_back(i);
            if (f(f, i-1, nr)) return true;
            tmp.pop_back();
        }
        if (dp[i][r]){
            if (f(f, i-1, r)) return true;
        }
        return false;
    };
    f(f, n-1, r);
    return 0;
}