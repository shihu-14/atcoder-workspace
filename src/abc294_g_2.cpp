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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
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
// オイラーツアーとセグ木を用いてLCAを求める。
struct LCA{
    vector<pair<int,int>> euler_tour;
    vector<int> first_appear, dist;
    vector<vector<int>> g;
    segtree<S, op, e> seg;
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
        for(int i=0; i< euler_tour.size(); i++) seg.set(i, euler_tour[i]);
    }
    // LCAを求める。
    int lca(int a, int b){
        int l = first_appear[a], r = first_appear[b];
        if (l > r) swap(l, r);
        return seg.prod(l, r+1).second;
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
    vector<T> edge;
    LCA g(n);
    map<pii, int> id;
    rep(i, n-1){
        int a, b, c; cin >> a >> b >> c; a--, b--;
        edge.emplace_back(a, b, c);
        g.add_edge(a, b);
        id[{a, b}] = id[{b, a}] = c;
    }
    g.init(0);
    map<pii, int> mp;
    rep(i, 2*n-2){
        mp[{g.euler_tour[i].se, g.euler_tour[i+1].se}] = i+1;
    }

    int q; cin >> q;
    fenwick_tree<ll> fw(2*n-1);
    rep(i, 2*n-2){
        int u = g.euler_tour[i].se, v = g.euler_tour[i+1].se;
        int w = id[{u, v}];
        int j1 = mp[{u, v}], j2 = mp[{v, u}];
        if (j1 > j2) swap(j1, j2);
        if (j1 == i+1) fw.add(i+1, w);
        else if (j2 == i+1) fw.add(i+1, -w);
    }
    rep(i, q){
        int type; cin >> type;
        if (type == 1){
            int j, w; cin >> j >> w; j--;
            auto [u, v, c] = edge[j];
            int k1 = mp[{u, v}], k2 = mp[{v, u}];
            if (k1 > k2) swap(k1, k2);
            fw.add(k1, -fw.sum(k1, k1+1)+w);
            fw.add(k2, -fw.sum(k2, k2+1)-w);
        }
        else{
            int u, v; cin >> u >> v; u--; v--;
            int lca = g.lca(u, v);
            ll ans = fw.sum(0, g.first_appear[u]+1)+fw.sum(0, g.first_appear[v]+1)-2*fw.sum(0, g.first_appear[lca]+1);
            cout << ans << endl;
        }
    }

    return 0;
}