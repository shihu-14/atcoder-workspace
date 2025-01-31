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
    int n, x; cin >> n >> x;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    rep(i, n){
        if (a[i] != x) cout << a[i] << " ";
    }
    cout << endl;
    return 0;
}