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
    vector<T> q;
    rep(i, n){
        int a, p, x;
        cin >> a >> p >> x;
        q.emplace_back(p, a, x);
    }
    sort(q.begin(), q.end());
    int ans = 1001001001;
    rep(i, n){
        auto [p, a, x] = q[i];
        if (a < x){
            ans = min(ans, p);
        }
    }
    cout << (ans == 1001001001 ? -1 : ans) << endl;
    return 0;
}