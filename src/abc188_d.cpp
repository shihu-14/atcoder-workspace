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
    int n; ll C; cin >> n >> C;
    vector<T> p;
    vector<int> c(n);
    rep(i, n){
        int a, b; cin >> a >> b >> c[i]; b++;
        p.emplace_back(a, 0, i);
        p.emplace_back(b, 1, i);
    }

    sort(p.begin(), p.end());
    ll ans = 0, sum = 0;
    rep(i, 2*n-1){
        auto [d1, t1, id1] = p[i];
        auto [d2, t2, id2] = p[i+1];
        if (t1 == 0){
            sum += c[id1];
        }
        else{
            sum -= c[id1];
        }
        ans += min(sum, C)*(d2-d1);
    }
    cout << ans << endl;
    return 0;
}   