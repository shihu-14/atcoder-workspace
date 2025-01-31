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
    vector<vector<int>> a(n), b(n), c(n);
    rep(i, n){
        int av; cin >> av; av--;
        a[av].push_back(i);
    }
    rep(i, n){
        int bv; cin >> bv; bv--;
        b[bv].push_back(i);
    }
    rep(i, n){
        int cv; cin >> cv; cv--;
        c[cv].push_back(i);
    }
    vector<int> cnt_b(n);
    rep(i, n){
        rep(j, b[i].size()){
            cnt_b[i] += c[b[i][j]].size();
        }
    }

    ll ans = 0;
    rep(i, n){
        ans += (ll)a[i].size() * cnt_b[i];
    }
    cout << ans << endl;
    return 0;
}