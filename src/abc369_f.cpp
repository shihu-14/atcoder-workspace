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
using S = pii;
S op(S a, S b){return max(a, b);}
S e() {return {0, -1};}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w, n; cin >> h >> w >> n;
    vector<pii> p;
    p.emplace_back(0, 0);
    rep2(i, 1, n+1){
        int r, c; cin >> r >> c; r--, c--;
        p.emplace_back(r, c);
    }
    sort(rng(p));
    segtree<S, op, e> seg(w); seg.set(0, {0, 0});
    vector<int> par(n+1, -1);
    rep2(i, 1, n+1){
        auto [r, c] = p[i];
        auto [mx, id] = seg.prod(0, c+1);
        par[i] = id;
        seg.set(c, {mx+1, i});
    }

    auto [res, id] = seg.prod(0, w);
    string ans;
    int nr = h-1, nc = w-1;
    while(nr > 0 || nc > 0){
        auto [r, c] = p[id]
        while(nr > r) nr--, ans += 'D';
        while(nc > c) nc--, ans += 'R';
        id = par[id];
    }

    reverse(rng(ans));
    cout << res << endl;
    cout << ans << endl;
    return 0;
}