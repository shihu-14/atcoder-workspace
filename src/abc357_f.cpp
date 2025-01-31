// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

struct S{
    mint a, b, c, s;
};
struct F{
    mint x, y;
};

S op(S x, S y){
    x.a += y.a;
    x.b += y.b;
    x.s += y.s;
    x.c += y.c;
    return x;
}

S e(){return {0, 0, 0, 0};}

S mapping(F f, S x){
    x.s += f.x*x.b+f.y*x.a+f.x*f.y*x.c;
    x.a += f.x*x.c;
    x.b += f.y*x.c;
    return x;
}

F composition(F f, F g){
    f.x += g.x;
    f.y += g.y;
    return f;
}

F id(){return {0, 0};}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n), b(n);
    rep(i,n) cin >> a[i];
    rep(i,n) cin >> b[i];
    lazy_segtree<S, op, e, F, mapping, composition, id> seg(n);
    rep(i, n) seg.set(i, {a[i], b[i], 1, (mint)a[i]*b[i]});
    rep(qi, q){
        int type; cin >> type;
        if (type == 1){
            int l, r, x; cin >> l >> r >> x; l--;
            seg.apply(l, r, {x, 0});
        }
        else if (type == 2){
            int l, r, x; cin >> l >> r >> x; l--;
            seg.apply(l, r, {0, x});
        }
        else{
            int l, r; cin >> l >> r; l--;
            cout << seg.prod(l, r).s.val() << '\n';
        }
    }
    cout.flush();
    return 0;
}