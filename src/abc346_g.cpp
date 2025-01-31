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
#define popcount_ll __builtin_popcountll
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
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;

struct Rect{
    int l1, r1, l2, r2;
    Rect(int l1=0, int r1=0, int l2=0, int r2=0): l1(l1), r1(r1), l2(l2), r2(r2) {}
};
namespace UnionOfRectangle{
    pii op(pii a, pii b){
        if (a.fi == b.fi) return {a.fi, a.se+b.se};
        else return min(a, b);
    }
    pii e(){ return {0, 0};}
    pii mapping(int f, pii x){
        x.fi += f;
        return x;
    }
    int composition(int f, int g){return f+g;}
    int id(){return 0;}
    struct Query{
        int r1, r2, c;
        Query(int r1=0, int r2=0, int c=0): r1(r1), r2(r2), c(c) {}
    };
    ll solve(int n, vector<Rect> rs){
        vector<vector<Query>> query(n+1);
        for (auto [l1, l2, r1, r2]: rs){
            query[l1].emplace_back(r1, r2, 1);
            query[l2].emplace_back(r1, r2, -1);
        }
        ll res = 0;
        lazy_segtree<pii, op, e, int, mapping, composition, id> seg(n);
        rep(i, n) seg.set(i, {0, 1});
        rep(qi, n+1){
            for (auto [r1, r2, c]: query[qi]) seg.apply(r1, r2, c);
            auto [mn, s] = seg.all_prod();
            if (mn == 0) res += n-s;
            else res += n;
        }
        return res;
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    vector<vector<int>> g(n);
    rep(i, n){
        cin >> a[i], a[i]--;
        g[a[i]].push_back(i);
    }
    vector<Rect> rs;
    rep(i, n){
        int l = lower_bound(rng(g[a[i]]), i) - g[a[i]].begin(); l--;
        int r = upper_bound(rng(g[a[i]]), i) - g[a[i]].begin();
        if (l < 0) l = 0;
        else l = g[a[i]][l]+1;
        if (r == g[a[i]].size()) r = n;
        else r = g[a[i]][r];
        rs.emplace_back(l, i+1, i, r);
    }
    ll ans = UnionOfRectangle::solve(n, rs);
    cout << ans << endl;
    return 0;
}

