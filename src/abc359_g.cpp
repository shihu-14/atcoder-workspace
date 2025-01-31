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

struct CentroidDecomposition{
    int n;
    vector<vector<int>> g;
    vector<bool> centroid_used;
    vector<int> subtree_size;
    // Edit here (define required data structures)
    ll ret = 0;
    vector<int> a;
    
    CentroidDecomposition(int n): n(n), g(n), centroid_used(n), subtree_size(n), a(n){}
    void add_edge(int u, int v){
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    void add_data(){
        // Edit here (input required data)
        rep(i, n) cin >> a[i];
    }
    void CD(int u){
        auto getCentroid = [&]() -> int{
            auto dfs = [&](auto f, int u, int p=-1) -> int{ // calc subtree size
                subtree_size[u] = 1;
                for (int v: g[u]){
                    if (v == p || centroid_used[v]) continue;
                    subtree_size[u] += f(f, v, u);
                }
                return subtree_size[u];
            };
            int m = dfs(dfs, u), centorid = -1;
            auto dfs2 = [&](auto f, int u, int p=-1) -> void{ // find centroid
                bool flag = true;
                if (m-subtree_size[u] > m/2) flag = false;
                for (int v: g[u]){
                    if (v == p || centroid_used[v]) continue;
                    f(f, v, u);
                    if (subtree_size[v] > m/2) flag = false;
                }
                if (flag){
                    centorid = u;
                    return;
                }
            };
            dfs2(dfs2, u);
            return centorid;
        };
        int centorid = getCentroid();
        centroid_used[centorid] = true;

        // Edit here (describe the process for the centroid-decomposed subtree)
        /* sample abc359-G
        map<int, ll> mc, ms;
        mc[a[centorid]] = 1, ms[a[centorid]] = 0;
        for (int v: g[centorid]){
            if (centroid_used[v]) continue;
            vector<pii> ps;
            auto dfs = [&](auto f, int u, int p=-1, int d=1) -> void{
                ps.emplace_back(a[u], d);
                for (int v: g[u]){
                    if (v == p || centroid_used[v]) continue;
                    f(f, v, u, d+1);
                }
            };
            dfs(dfs, v);
            for (auto [c, d]: ps){
                ret += ms[c]+mc[c]*d;
            }
            for (auto [c, d]: ps){
                mc[c]++;
                ms[c] += d;
            }
        }
        */
        for (int v: g[centorid]){ // decompose about the centroid
            if (centroid_used[v]) continue;
            CD(v);
        }
    }
    ll solve(){
        CD(0);
        return ret;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    CentroidDecomposition g(n);
    rep(i, n-1){
        int u, v; cin >> u >> v; u--, v--;
        g.add_edge(u, v);
    }
    g.add_data();
    cout << g.solve() << endl;
    return 0;
}