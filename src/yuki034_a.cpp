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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, H, sx, sy, gx, gy; cin >> n >> H >> sy >> sx >> gy >> gx; sx--, sy--, gx--, gy--;
    vector<vector<pii>> g(n*n);
    rep(i, n)rep(j, n){
        int l; cin >> l;
        rep(t, 4){
            int nx = i+dx[t], ny = j+dy[t];
            if(nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
            g[nx*n+ny].emplace_back(i*n+j, l);
        }
    }
    if (H > 18*(n-1)){
        cout << abs(sx-gx)+abs(sy-gy) << endl;
        return 0;
    }

    vector dist(n*n, vector<int>(H, INF)); dist[sx*n+sy][0] = 0;
    priority_queue<T, vector<T>, greater<T>> pq;
    pq.emplace(0, 0, sx*n+sy);
    while(!pq.empty()){
        auto [t, h, u] = pq.top(); pq.pop();
        if (dist[u][h] < t) continue;
        for(auto [v, w]: g[u]){
            if (h+w >= H) continue;
            if(dist[v][h+w] > dist[u][h]+1){
                dist[v][h+w] = dist[u][h]+1;
                pq.emplace(dist[v][h+w], h+w, v);
            }
        }
    }
    int ans = INF;
    rep(i, H) chmin(ans, dist[gx*n+gy][i]);
    if (ans == INF) cout << -1 << endl;    
    else cout << ans << endl;
    return 0;
}