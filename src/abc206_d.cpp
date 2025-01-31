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
    vector<int> a(n), b(200010);
    rep(i, n) cin >> a[i];
    rep(i, 200010) b[i] = i;
    auto f = [&](auto f, int now) -> int{
        if (b[now] == now) return now;
        return b[now] = f(f, b[now]);
    };
    int ans = 0;
    rep(i, n/2){
        int x = f(f, a[i]), y = f(f, a[n-i-1]);
        if (x == y) continue;
        b[x] = y;
        ans++;
    }
    cout << ans << endl;
    return 0;
}