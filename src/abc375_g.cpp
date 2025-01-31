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
using T = tuple<int, int, int, int>;
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

// 無向連結グラフにおける橋と関節点の検出を行う構造体
struct Bridge{
    int n;
    vector<int> ord, low;
    vector<bool> art;
    vector<vector<int>> g;
    Bridge(int n) : n(n), ord(n, -1), low(n), art(n), g(n) {}
    void add_edge(int a, int b){
        g[a].emplace_back(b);
        g[b].emplace_back(a);
    }
    void dfs(int u, int p, int &c){
        low[u] = ord[u] = c++; // dfsで見た順番を記録。
        int t = 0; // 無向辺を正しく処理するための変数。
        bool isart = false; // 関節点かどうか
        for(auto v: g[u]){
            if (ord[v] == -1){
                dfs(v, u, c);
                chmin(low[u], low[v]);
                isart |= ord[v] <= low[u];
            } 
            else if (v != p){ // 後退辺によるlowの更新
                chmin(low[u], ord[v]);
            }
        }
        art[u] = isart;
    }
    void build(){
        int c = 0;
        rep(u, n) if (ord[u] == -1){
            int num_child = 0;
            low[u] = ord[u] = c++;
            for (auto v: g[u]) if (ord[v] == -1){ // uを根として、uの子であるvについてdfsを行う。
                dfs(v, u, c);
                num_child++;
            }
            if (num_child >= 2) art[u] = true; // 根である場合の関節点判定
        }
    }
    bool isbridge(int a, int b){
        if (ord[a] > ord[b]) swap(a, b);
        return ord[a] < low[b];
    }
    bool isart(int v){return art[v];}
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<pll>> g(n);
    vector<T> edge; 
    rep(i, m){
        int a, b; ll c; cin >> a >> b >> c;
        a--, b--;
        g[a].emplace_back(b, c);
        g[b].emplace_back(a, c);
        edge.emplace_back(a, b, c, i);
    }

    auto f = [&](int start) -> vector<ll>{
        priority_queue<pll, vector<pll>, greater<pll>> pq;
        pq.push({0, start});
        vector<ll> dist(n, LINF);
        dist[start] = 0;
        while(!pq.empty()){
            auto [d, u] = pq.top(); pq.pop();
            if(dist[u] < d) continue;
            for(auto [v, c]: g[u]){
                if (dist[v] > dist[u]+c){
                    dist[v] = dist[u]+c;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    };

    auto dist1 = f(0);
    auto dist2 = f(n-1);

    Bridge g2(n);
    vector<bool> onpass(m);
    for (auto [u, v, c, id]: edge){
        if (dist1[u]+c+dist2[v] == dist1[n-1]){
            onpass[id] = true;
            g2.add_edge(u, v);
        }
        swap(u, v);
        if (dist1[u]+c+dist2[v] == dist1[n-1]){
            onpass[id] = true;
            g2.add_edge(u, v);
        }
    }
    g2.build();
    rep(i, m){
        auto [u, v, c, id] = edge[i];
        if (onpass[i]){
            if (g2.isbridge(u, v)) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
        else{
            cout << "No" << endl;
        }
    }
    return 0;
}