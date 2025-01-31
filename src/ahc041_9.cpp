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
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;
int n, m, h; 
const int bigger_limit= 30;
vector<int> calc(G &g, vector<int> &a, vector<int> &que, int &score){
    vector<bool> used(n);
    vector<int> par(n, -1);
    auto dfs = [&](auto f, int u, int d=0) -> void{
        used[u] = true;
        score += (1+d)*a[u];
        if (d >= h){
            return;
        }
        for (auto v: g[u]){
            if (used[v]) continue;
            par[v] = u;
            f(f, v, d+1);
        }
    };
    for (auto i: que){
        if (used[i]) continue;
        dfs(dfs, i);
    }
    return par;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = std::chrono::high_resolution_clock::now();
    cin >> n >> m >> h;
    G g(n);
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    rep(i, m){
        int u, v; cin >> u >> v;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    rep(i, n){ int x, y; cin >> x >> y;}
    // final ouput
    int max_score = 0;
    vector<int> final_par, que(n);
    rep(i, n) que[i] = i;
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, n-1);
    int times = 0, update = 0;
    // 
    while(1){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() > 1975) break;
        times++;
        // tmp
        int tmp_score = 0;
        vector<int> tmp_que = que;
        G tmp_g = g;
        rep(t, 150){
            int j1 = dist(gen);
            int j2 = dist(gen);
            swap(tmp_que[j1], tmp_que[j2]);
        }
        rep(t, 15){
            int j = dist(gen);
            if (g[j].size() < 2) continue;
            std::uniform_int_distribution<> dist2(0, g[j].size()-1);
            int j1 = dist2(gen);
            int j2 = dist2(gen);
            swap(tmp_g[j][j1], tmp_g[j][j2]);
        }
        
        vector<int> par = calc(tmp_g, a, tmp_que, tmp_score);
        if (tmp_score > max_score){
            max_score = tmp_score;
            final_par = par;
            que = tmp_que;
            g = tmp_g;
            update++;
        }
    }
    cerr << "times: " << times << endl;
    cerr << "update: " << update << endl;
    rep(i, n) cout << final_par[i] << " ";
    cout << endl;
    return 0;
}