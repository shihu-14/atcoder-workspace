#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const int INF = 1e9;
int main(){
    int n, m; cin >> n >> m;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    int ans = INF;
    vector<int> c(n);
    set<int> in, out;
    rep(i, n+1) out.insert(i);
    rep(i, m){
        c[a[i]]++;
        in.insert(a[i]);
        out.erase(a[i]);
    }
    ans = min(ans, *out.begin());
    for(int i = m; i < n; i++){
        c[a[i]]++;
        if (c[a[i]] == 1){
            in.insert(a[i]);
            out.erase(a[i]);
        }
        c[a[i-m]]--;
        if (c[a[i-m]] == 0){
            in.erase(a[i-m]);
            out.insert(a[i-m]);
        }
        ans = min(ans, *out.begin());
    }
    cout << ans << endl;
    return 0;
}