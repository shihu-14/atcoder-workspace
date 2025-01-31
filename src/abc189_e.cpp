#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<ll, ll>;
using T = tuple<int, int, int>;
using S = ll;
struct F{
    ll d, c;
};
S op(S a, S b) { return max(a, b); }
S e() { return 0; }
S mapping(F f, S x) { return f.c*x+f.d; }
F composition(F f, F g) { return {f.d+g.d*f.c, f.c*g.c};}
F id() { return {0, 1}; }
int main(){
    int n, m, Q; cin >> n;
    vector<P> p;
    lazy_segtree<S, op, e, F, mapping, composition, id> segx(n), segy(n);
    rep(i, n){
        ll x, y; cin >> x >> y;
        p.emplace_back(x, y);
        segx.set(i, x);
        segy.set(i, y);
    }
    cin >> m;
    vector<P> op;
    rep(i, m){
        int t; cin >> t;
        if (t < 3){
            op.emplace_back(t, 0);
        }
        else{
            ll pi; cin >> pi;
            op.emplace_back(t, pi);
        }
    }
    cin >> Q;
    vector<T> q;
    rep(i, Q){
        int a, b; cin >> a >> b; b--;
        q.emplace_back(a, b, i);
    }
    sort(q.begin(), q.end());
    
    vector<P> ans(Q);
    int c = 0, f = 0;
    rep(i, Q){
        auto [a, b, id] = q[i];
        if (a <= c){
            ll x = segx.get(b), y = segy.get(b);
            if (f) swap(x, y);
            ans[id] = {x, y};
            continue;
        }
        while(c < a){
            auto [t, pi] = op[c];
            if (t == 1){
                if (f) segy.apply(0, n, {0, -1});
                else segx.apply(0, n, {0, -1});
                f ^= 1;
            }
            else if (t == 2){
                if (f) segx.apply(0, n, {0, -1});
                else segy.apply(0, n, {0, -1});
                f ^= 1;
            }
            else if (t == 3){
                if (f) segy.apply(0, n, {2*pi, -1});
                else segx.apply(0, n, {2*pi, -1});
            }
            else{
                if (f) segx.apply(0, n, {2*pi, -1});
                else segy.apply(0, n, {2*pi, -1});
            }
            c++;
        }
        ll x = segx.get(b), y = segy.get(b);
        if (f) swap(x, y);
        ans[id] = {x, y};
    }
    rep(i, Q){
        cout << ans[i].first << " " << ans[i].second << endl;
    }
    return 0;
}