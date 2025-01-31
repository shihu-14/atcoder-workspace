#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

ll gcd(ll a, ll b){
    return (b ? gcd(b, a%b) : a);
}

// ax+by=gとなるg=gcd(a, b), x, yを求める拡張gcd
tuple<ll, ll, ll> extgcd(ll a, ll b) {
    if (b == 0) return {a, 1, 0};
    ll g, x, y;
    tie(g, x, y) = extgcd(b, a%b);
    return {g, y, x-a/b*y};
}

int main(){
    int t; cin >> t;
    rep(i, t){
        ll n, s, k; cin >> n >> s >> k;
        auto [g, x, y] = extgcd(k, n);
        ll d = n-s;
        if (d%g != 0){
            cout << -1 << endl;
            continue;
        }
        d /= g;
        n /= g;
        k /= g;
        ll ans = ((x*d)%n+n)%n;
        cout << ans << endl;

    }
    return 0;
}