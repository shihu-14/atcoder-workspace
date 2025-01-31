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
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;
// 木構造に関する操作 (未完成)
struct Tree{
    struct Edge{
        int to, cost;
        Edge() {}
        Edge(int to, int cost) : to(to), cost(cost) {}
    };
    int n;
    ll sum=0;
    vector<int> deg;
    vector<int> par;
    vector<pii> tour;
    vector<ll> dist;
    vector<ll> subNode;
    vector<pii> lr;
    vector<vector<Edge>> g;
    vector<ll> weight;
    Tree(int n): n(n), g(n), deg(n), par(n), dist(n), lr(n), subNode(n){}
    void add_edge(int u, int v, int c){
        g[u].emplace_back(v, c);
        g[v].emplace_back(u, c);
        deg[u]++, deg[v]++;
    }
    // 頂点startを根としたオイラーツアー、各頂点の根からの距離、親頂点を求める。
    void setting(int start, vector<ll> &w){
        ll s = 0;
        auto f = [&](auto f, int u, int p=-1) -> void {
            dist[u] = s;
            lr[u].fi = tour.size();
            tour.emplace_back(u, 1);
            for (auto [v, w]: g[u]){
                if (v == p){
                    par[u] = v;
                }
                else{
                    s += w;
                    f(f, v, u);
                    subNode[u] += subNode[v];
                    tour.emplace_back(u, 0);
                    s -= w;
                }
            }
            subNode[u]++;
            lr[u].se = tour.size();
            return;
        };
        f(f, start);
        weight = w;
        rep(i, n) sum += w[i];
    }
    // 木の直径を返す
    int find_diameter(){
        int from = 0, to = 0; ll max_dist = -LINF;
        auto f = [&](auto f, int u, int p=-1, int s=0) -> void{
            if (g[u].size() == 1 && p != -1){
                if (max_dist < s){
                    max_dist = s;
                    to = u;
                }
                return;
            }
            for (auto [v, c]: g[u]) if (v != p){
                f(f, v, u, s+c);
            }   
        };
        f(f, from); from = to;
        f(f, from);
        return max_dist;
    }
    // 木の中心を返す(高々２つなので、１つの場合は片方は-1となる)
    pii find_center(){
        int res_n = n;
        vector<int> tmp_deg = deg;
        vector<bool> used(n);
        queue<int> q;
        rep(i, n) if (tmp_deg[i] == 1) q.emplace(i); // 次数が1の頂点でキューを初期化
        while(res_n > 2){
            res_n -= q.size();
            queue<int> tmp_q;
            while(!q.empty()){
                int u = q.front(); q.pop();
                used[u] = true;
                for (auto [v, c]: g[u]){ // 葉を取り除いたことで次数が1になった頂点をキューに入れる
                    tmp_deg[v]--;
                    if (tmp_deg[v] == 1) tmp_q.emplace(v);
                }
            }
            q = move(tmp_q);
        }
        vector<int> res;
        rep(i, n) if (!used[i]) res.emplace_back(i);
        return (res.size() == 1 ? make_pair(res[0], -1) : make_pair(res[0], res[1]));
    }
    // 木の重心を１つ求める(本来は高々２つある)
    int find_centroid(){
        int centroid = -1;
        vector<ll> sv(n);
        auto f = [&](auto f, int u, int p=-1) -> ll{
            ll mx = 0;
            for (auto [v, c]: g[u]) if (v != p){
                sv[u] += f(f, v, u);
                chmax(mx, sv[v]);
            }
            sv[u] += weight[u];
            chmax(mx, sum-sv[u]);
            if (mx*2 <= sum) centroid = u;
            return sv[u];
        };
        f(f, 0);
        return centroid;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    Tree T(n); 
    rep(i, n-1){
        int a, b; cin >> a >> b; a--, b--;
        T.add_edge(a, b, 1);
    }
    vector<ll> c(n);
    rep(i, n) cin >> c[i];
    T.setting(0, c);
    ll ans = 0;
    auto f = [&](auto f, int u, int p=-1, int d=0) -> void {
        ans += d*c[u];
        for (auto [v, _]: T.g[u]){
            if (v == p) continue;
            f(f, v, u, d+1);
        }
    };
    f(f, T.find_centroid());
    cout << ans << endl;
    return 0;
}