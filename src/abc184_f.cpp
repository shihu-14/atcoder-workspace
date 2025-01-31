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
    int n, t; cin >> n >> t;
    vector<int> a(n/2), b(n-n/2);
    rep(i, n/2) cin >> a[i];
    rep(i, n-n/2) cin >> b[i];

    int n1 = 1<<(n/2), n2 = 1<<(n-n/2);
    vector<ll> s1, s2;
    rep(i, n1){
        ll sum = 0;
        rep(j, n/2){
            if (i&(1<<j)) sum += a[j];
        }
        s1.push_back(sum);
    }
    rep(i, n2){
        ll sum = 0;
        rep(j, n-n/2){
            if (i&(1<<j)) sum += b[j];
        }
        s2.push_back(sum);
    }
    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());

    ll ans = 0;
    rep(i, n1){
        int it = lower_bound(s2.begin(), s2.end(), t-s1[i]) - s2.begin();
        if (it == 0 && s1[i]+s2[it] <= t){
            ans = max(ans, s1[i]+s2[it]);
        }
        else if (it == n2 && s1[i]+s2[it-1] <= t){
            ans = max(ans, s1[i]+s2[it-1]);
        }
        else{
            if (s1[i]+s2[it] <= t) ans = max(ans, s1[i]+s2[it]);
            if (s1[i]+s2[it-1] <= t) ans = max(ans, s1[i]+s2[it-1]);
        }
    }
    cout << ans << endl;
    return 0;
}