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
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

// オイラーツアーとセグ木を用いてLCAを求める。
struct LCA{
    vector<S> euler_tour;
    vector<int> first_appear, dist;
    vector<vector<int>> g;
    segtree<S, op, e> seg;
    SparseTable<S> st;
    LCA(int n) : first_appear(n), dist(n), g(n), seg(2*n-1){}
    void add_edge(int a, int b){
        g[a].push_back(b);
        g[b].push_back(a);
    }
    // オイラーツアーを求める。
    void dfs(int now, int pre=-1, int d=0){
        first_appear[now] = euler_tour.size();
        dist[now] = d;
        euler_tour.emplace_back(d, now);
        for (auto to: g[now]){
            if (to == pre) continue;
            dfs(to, now, d+1);
            euler_tour.emplace_back(d, now);
        }
    };
    // 根を指定してオイラーツアー、seg木を初期化する。
    void init(int root){
        dfs(root);
        st.build(euler_tour, op);
        // for(int i=0; i< euler_tour.size(); i++) seg.set(i, euler_tour[i]);
    }
    // LCAを求める。
    int lca(int a, int b){
        int l = first_appear[a], r = first_appear[b];
        if (l > r) swap(l, r);
        return st.query(l, r+1).second;
    }
    // 任意の2点間の距離を求める。
    int length(int a, int b){return dist[a] + dist[b] - 2*dist[lca(a, b)];}
    // ある点が指定したパス上(a,b)にあるかどうかの判定をする。
    bool is_on_pass(int a, int b, int c){return length(a, b) == length(a, c)+length(c, b);}
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    rep(i, n) cin >> a[i], a[i]--;
    G g(n);
    rep(i, n-1){
        int u, v; cin >> u >> v; u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    return 0;
}