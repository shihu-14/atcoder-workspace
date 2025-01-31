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
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
int target;
struct S{
    int x, size;
    ll sum;
};
S op(S a, S b){
    S res;
    res.x = min(a.x, b.x);
    res.sum = a.sum + b.sum;
    res.size = a.size + b.size;
    return res;
}
S e(){
    return {INF, 0, 0};
}
using F = int;
S mapping(F f, S x){
    if (f == -1) return x;
    S res;
    res.x = f;
    res.size = x.size;
    res.sum = (ll)f*x.size;
    return res;
}
F composition(F f, F g){
    if (f == -1) return g;
    return f;
}
F id(){return -1;}
bool f(S x){return target <= x.x;}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n;
    lazy_segtree<S, op, e, F, mapping, composition, id> seg(n);
    rep(i, n){
        int x; cin >> x; x -= i;
        seg.set(i, {x, 1, x});
    }
    ll ans = 0;
    cin >> q;
    rep(i, q){
        int t, g; cin >> t >> g; t--; g -= t;
        target = g;
        ll res = 0;
        int l = seg.min_left<f>(n);
        if (seg.get(t).x < g){
            auto [_, size, sum] = seg.prod(t, l);
            res += (ll)size*g-sum;
            seg.apply(t, l, g);
        }
        else{
            auto [_, size, sum] = seg.prod(l, t+1);
            res += sum-(ll)size*g;
            seg.apply(l, t+1, g);
        }
        ans += res;
    }
    cout << ans << endl;
    return 0;
}