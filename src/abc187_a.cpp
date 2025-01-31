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
    int a, b; cin >> a >> b;
    string sa = to_string(a), sb = to_string(b);
    int ansa = 0, ansb = 0;
    rep(i, 3){
        ansa += sa[i] - '0';
        ansb += sb[i] - '0';
    }
    cout << max(ansa, ansb) << endl;
    return 0;
}