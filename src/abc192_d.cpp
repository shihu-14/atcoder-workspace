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
    string x; cin >> x;
    ll m; cin >> m;
    int n = x.size();
    if (n == 1){
        if (x[0]-'0' <= m) cout << 1 << endl;
        else cout << 0 << endl;
        return 0;
    }
    int md = 0;
    rep(i, n){
        md = max(md, x[i]-'0');
    }
    auto f = [&](ll d){
        ll res = 0, b = 1;
        for(int i = n-1; i >= 0; i--){
            if (x[i] != '0' && (m-res)/(b*(x[i]-'0')) < 1){
                return false;
            }
            res += (x[i]-'0')*b;
            if (i != 0 && b > m/d) return false;
            b *= d;
        }
        return res <= m;
    };
    ll ac = md+1, wa = m+1;
    while(wa-ac > 1){
        ll wj = (ac+wa)/2;
        if (f(wj)) ac = wj;
        else wa = wj;
    }
    if (!f(ac)) cout << 0 << endl;
    else cout << ac-md << endl;
    return 0;
}