#include <iostream>
#include <vector>
#include <string>
using namespace std;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
const ll MOD = 998244353;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size();
    vector<vector<ll>> dp2(n, vector<ll>(n+1));
    rep(i, n) dp2[i][i] = 1, dp2[i][i+1] = (s[i] == '?' ? 26 : 1);
    rep(w, n){
        rep(l, n){
            int r = l+w;
            if (l && r < n){
                if (s[l-1] == '?' && s[r] == '?') dp2[l-1][r+1] += (26*dp2[l][r])%MOD;
                else if (s[l-1] == '?') dp2[l-1][r+1] += dp2[l][r];
                else if (s[r] == '?') dp2[l-1][r+1] += dp2[l][r];
                else if (s[l-1] == s[r]) dp2[l-1][r+1] += dp2[l][r];
                dp2[l-1][r+1] %= MOD;
            }
        }
    }

    ll ans = 0;
    vector<ll> c(n+1), a(n+1, 1);
    rep(i, n) c[i+1] = s[i] == '?';
    rep(i, n) c[i+1] += c[i];
    rep(i, n) a[i+1] *= (a[i]*26)%MOD, a[i+1] %= MOD;
    rep(i, n){
        rep2(j, i+1, n+1){
            ans += (a[(c[n]-(c[j]-c[i]))]*dp2[i][j])%MOD;
            ans %= MOD;
        }
    }
    ans %= MOD;
    cout << ans << endl;
    return 0;
}