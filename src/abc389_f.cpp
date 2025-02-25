// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
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
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}
int target;
int op(int a, int b){return max(a, b);}
int e(){return 0;}
int mapping(int f, int x){return f+x;}
int composition(int f, int g){return f+g;}
int id(){return 0;}
bool f(int x){return x < target;}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n;
    vector<pii> p;
    rep(i, n){
        int l, r; cin >> l >> r;
        p.emplace_back(l, r);
    }
    cin >> q;
    vector<pii> query;
    rep(qi, q){
        int x; cin >> x;
        query.emplace_back(x, qi);
    }
    sort(rng(query));
    lazy_segtree<int, op, e, int, mapping, composition, id> seg(q);
    rep(i, q) seg.set(i, query[i].fi);
    for (auto [l, r]: p){
        target = l;
        auto L = seg.max_right<f>(0);
        target = r+1;
        auto R = seg.max_right<f>(0);
        seg.apply(L, R, 1);
    }
    vector<int> ans(q);
    rep(i, q){
        ans[query[i].se] = seg.get(i);
    }
    rep(i, q) cout << ans[i] << endl;
    return 0;
}
