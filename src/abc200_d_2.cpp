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

    vector<vector<int>> c(200);
    n = (n >= 8 ? 8 : n); 
    for (int i = 1; i < 1<<n; i++){
        int tmp = 0;
        rep(j, n){
            if (i&(1<<j)) tmp = (tmp + a[j])%200;
        }
        c[tmp].push_back(i);
    }

    rep(i, 200){
        if (c[i].size() >= 2){
            cout << "Yes" << endl;
            cout << __builtin_popcount(c[i][0]) << " ";
            rep(j, n) if (c[i][0]&(1<<j)) cout << j+1 << " ";
            cout << endl;
            cout << __builtin_popcount(c[i][1]) << " ";
            rep(j, n) if (c[i][1]&(1<<j)) cout << j+1 << " ";
            cout << endl;
            return 0;
        }
    }
    cout << "No" << endl;
    return 0;
}