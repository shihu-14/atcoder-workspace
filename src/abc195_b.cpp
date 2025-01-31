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
    int a, b, w; cin >> a >> b >> w;
    int mink = 1e9, maxk = -1;
    rep(i, 1000001){
        if (a*i <= 1000*w && 1000*w <= b*i){
            mink = min(mink, i);
            maxk = max(maxk, i);
        }
    }
    if (mink == 1e9 || maxk == -1) cout << "UNSATISFIABLE" << endl;
    else cout << mink << " " << maxk << endl;
    return 0;
}