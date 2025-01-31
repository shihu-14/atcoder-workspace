#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

using S = int;
S op(int a, int b){return a^b;}
S e(){return 0;}
int main(){
    int n, q; cin >> n >> q;
    segtree<S, op, e> seg(n);
    rep(i, n){
        int a; cin >> a;
        seg.set(i, a);
    }
    rep(i, q){
        int t, x, y; cin >> t >> x >> y;
        if (t == 1){    
            x--;
            int a = seg.get(x);
            a ^= y;
            seg.set(x, a);
        }
        else{
            x--;
            cout << seg.prod(x, y) << endl;
        }
    }
    return 0;
}