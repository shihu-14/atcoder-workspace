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
using T = tuple<int, int, ll>;
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, q; cin >> n >> m >> q;
    vector<T> edge; 
    rep(i, m){
        int u, v; ll t; cin >> u >> v >> t;
        u--; v--;
        edge.emplace_back(u, v, t);
    }

    vector<T> query;
    vector<bool> used(m);
    rep(i, q){
        int type; cin >> type;
        if (type == 1){
            int x; cin >> x; x--;
            used[x] = true;
            query.emplace_back(type, x, -1);
        }
        else{
            int x, y; cin >> x >> y;
            x--; y--;
            query.emplace_back(type, x, y);
        }
    }
    reverse(rng(query));
    vector<vector<ll>> dist(n, vector<ll>(n, LINF));
    rep(i, n) dist[i][i] = 0;
    rep(i, m){
        auto [u, v, t] = edge[i];
        if (used[i]) continue;
        dist[u][v] = dist[v][u] = t;
    }
    rep(k, n)rep(i, n)rep(j, n) chmin(dist[i][j], dist[i][k] + dist[k][j]);
    vector<ll> ans;
    for (auto [type, x, y] : query){
        if (type == 1){
            auto [u, v, t] = edge[x];
            dist[u][v] = dist[v][u] = min(dist[u][v], t);
            for (int k: {u, v})rep(i, n)rep(j, n){
                chmin(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
        else{
            ans.emplace_back((dist[x][y] == LINF ? -1 : dist[x][y]));
        }
    }
    reverse(rng(ans));
    rep(i, ans.size()) cout << ans[i] << endl; 
    return 0;
}