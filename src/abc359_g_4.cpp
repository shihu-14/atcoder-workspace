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
using S = pair<int, int>;
S op(S a, S b){return min(a, b);}
S e(){return {INF, 0};}

// Sparse Table 前処理 O(NlogN), クエリ O(1)
template<typename T>
struct SparseTable {
    int n;
    vector<vector<T>> table; // table[i][j] = dataの区間[i, i+2^j)の演算結果を格納
    vector<int> logtable; // floor(log2(i))を格納
    function<T(T,T)> op; // 2項演算子(結合性・冪等性を満たすもの)
    SparseTable() {}
    void build(const vector<T> &data, function<T(T,T)> f){
        n = data.size(); logtable.resize(n+1); op = f; // メンバ変数の初期化
        for(int i = 2; i <= n; i++) logtable[i] = logtable[i/2] + 1;
        int max_log = logtable[n];
        table.resize(n, vector<T>(max_log+1));
        for(int i = 0; i < n; i++) table[i][0] = data[i];
        for(int j = 1; j <= max_log; j++){
            for(int i = 0; i+(1<<(j-1)) < n; i++){
                if(i+(1<<j) > n) break;
                table[i][j] = op(table[i][j-1], table[i+(1<<(j-1))][j-1]);
            }
        }
    }
    // dataの区間[l, r)の演算結果を返す
    T query(int l, int r){
        int ql = r-l;
        assert(ql > 0 && ql <= n);
        int k = logtable[ql];
        return op(table[l][k], table[r-(1<<k)][k]);
    }
};

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
    G g(n);
    LCA g2(n);
    map<int, vector<int>> mpc;
    vector<int> a(n);
    rep(i, n-1){
        int u, v; cin >> u >> v; u--, v--;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
        g2.add_edge(u, v);
    }
    g2.init(0);
    rep(i, n) cin >> a[i], mpc[a[i]].emplace_back(i);
    int D = sqrt(n);
    vector<int> lower_c, upper_c;
    G lower, upper;
    for (auto [k, v]: mpc){
        if (v.size() < D){
            lower_c.emplace_back(k);
            lower.emplace_back(v);
        }
        else{
            upper_c.emplace_back(k);
            upper.emplace_back(v);
        }
    }

    ll ans = 0;
    rep(i, upper_c.size()){
        auto f = [&](auto f, int u, int p=-1) -> int{
            int res = (a[u] == upper_c[i]);
            for (int v: g[u]) if(v != p){
                res += f(f, v, u);
            }
            ans += (ll)res*(upper[i].size()-res);
            return res;
        };
        f(f, i);
    }
    rep(k, lower_c.size())rep(i, lower[k].size())rep2(j, i+1, lower[k].size()){
        ans += g2.length(lower[k][i], lower[k][j]);
    }
    cout << ans << endl;
    return 0;
}