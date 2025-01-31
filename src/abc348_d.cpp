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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    int sx, sy, gx, gy;
    rep(i, h)rep(j, w){
        if (g[i][j] == 'S') sx = i, sy = j;
        if (g[i][j] == 'T') gx = i, gy = j;
    }
    int n; cin >> n;
    vector<T> ene;
    rep(i, n){
        int r, c, e; cin >> r >> c >> e; r--, c--;
        ene.emplace_back(r, c, e);
    }
    ene.emplace_back(gx, gy, 0);
    auto f = [&](int sx, int sy) -> vector<vector<int>>{
        vector dist(h, vector<int>(w, INF)); dist[sx][sy] = 0;
        priority_queue<T, vector<T>, greater<T>> pq;
        pq.emplace(0, sx, sy);
        while(!pq.empty()){
            auto [d, x, y] = pq.top(); pq.pop();
            if(dist[x][y] < d) continue;
            for(int i = 0; i < 4; i++){
                int nx = x+dx[i], ny = y+dy[i];
                if (nx < 0 || nx >= h || ny < 0 || ny >= w || g[nx][ny] == '#') continue;
                if (dist[nx][ny] > dist[x][y]+1){
                    dist[nx][ny] = dist[x][y]+1;
                    pq.emplace(dist[nx][ny], nx, ny);
                }
            }
        }
        return dist;
    };
    G g2(h*w);
    rep(i, n+1){
        auto [x, y, e] = ene[i];
        auto res = f(x, y);
        rep(j, n+1) if (i != j) {
            auto [x2, y2, _] = ene[j];
            if (res[x2][y2] <= e){
                g2[x*w+y].emplace_back(x2*w+y2);
            }
        }
    }
    vector<bool> used(h*w);
    auto dfs = [&](auto f, int u) -> bool{
        if (gx*w+gy == u) return true;
        used[u] = true;
        for (auto v: g2[u]){
            if (used[v]) continue;
            if (f(f, v)) return true;
        }    
        return false;
    };
    if (dfs(dfs, sx*w+sy)) cout << "Yes" << endl;
    else cout << "No" << endl;
    return 0;
}