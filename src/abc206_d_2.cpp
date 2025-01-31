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
    int ans = 0;
    dsu uf(200010);
    rep(i, n/2){
        uf.merge(a[i], a[n-i-1]);
    }
    for(auto v: uf.groups()){
        ans += v.size()-1;
    }
    cout << ans << endl;
    return 0;
}