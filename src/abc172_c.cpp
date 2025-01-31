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
    int n, m, k; cin >> n >> m >> k;
    vector<ll> a(n), b(m);
    rep(i, n) cin >> a[i];
    rep(i, m) cin >> b[i];
    rep(i, m-1) b[i+1] += b[i]; 
    int ans = 0; ll time = 0;
    int tmp = lower_bound(b.begin(), b.end(), k) - b.begin();
    if (tmp < m && b[tmp] == k) tmp++;
    ans = max(ans, tmp);
    rep(i, n){
        time += a[i];
        if (time > k) break;
        int it = lower_bound(b.begin(), b.end(), k-time) - b.begin();
        if (it < m && b[it]+time == k) it++;
        ans = max(ans, it+(i+1));
    }
    cout << ans << endl;
    return 0;
}