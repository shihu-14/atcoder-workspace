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
    int n, m, t; cin >> n >> m >> t;
    int res = n;
    int preb = 0;
    rep(i, m){
        int a, b; cin >> a >> b;
        res -= (a-preb);
        if (res <= 0){
            cout << "No" << endl;
            return 0;
        }
        res += b-a;
        if (res > n) res = n;
        preb = b;
    }
    res -= (t-preb);
    if (res <= 0){
        cout << "No" << endl;
        return 0;
    }
    cout << "Yes" << endl;
    return 0;
}