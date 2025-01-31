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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    int sx = 0, sy = 0, gx = 0, gy = 0;
    {
        rep(i, h)rep(j, w){
            if (g[i][j] == 'S') sx = i, sy = j;
            if (g[i][j] == 'G') gx = i, gy = j;
        }
    } 

    vector dist(h, vector<vector<int>>(w, vector<int>(2, INF))); dist[sx][sy][0] = 0, dist[sx][sy][1] = 0;
    priority_queue<T, vector<T>, greater<T>> pq; pq.emplace(0, 0, sx, sy), pq.emplace(0, 1, sx, sy);
    while(!pq.empty()){
        auto [d, f, x, y] = pq.top(); pq.pop();
        if (dist[x][y][f] < d) continue;
        for (int i = 0; i < 4; ++i){
            int nx = x+dx[i], ny = y+dy[i];
            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if (g[nx][ny] == '#') continue;
            if (f == 0 && (i == 0 || i == 2)){
                if (dist[nx][ny][1] > d+1){
                    dist[nx][ny][1] = d+1;
                    pq.emplace(d+1, 1, nx, ny);
                }
            } 
            else if (f == 1 && (i == 1 || i == 3)){
                if (dist[nx][ny][0] > d+1){
                    dist[nx][ny][0] = d+1;
                    pq.emplace(d+1, 0, nx, ny);
                }
            }
        }
    }
    int ans = min(dist[gx][gy][0], dist[gx][gy][1]);
    cout << (ans == INF ? -1 : ans) << endl;
    return 0;
}