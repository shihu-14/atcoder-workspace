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
    sort(a.rbegin(), a.rend());
    ll ans = 0;
    map<int, int> mp;
    set<int>  st;
    ans += a[0];
    mp[a[1]] += 2;
    st.insert(a[1]);
    rep(i, n-2){
        int r = *st.rbegin();
        ans += r;
        mp[r]--;
        if (mp[r] == 0) st.erase(r);
        st.insert(a[i+2]);
        mp[a[i+2]] += 2;
    }
    cout << ans << endl;
    return 0;
}