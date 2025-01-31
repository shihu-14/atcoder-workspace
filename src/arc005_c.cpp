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
int dist[510][510][10];
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    int sx, sy, gx, gy;
    rep(i, h)rep(j, w){
        if (g[i][j] == 's') sx = i, sy = j;
        if (g[i][j] == 'g') gx = i, gy = j;
    }
    rep(i, h)rep(j, w)rep(k, 3) dist[i][j][k] = INF;
    rep(i, 3) dist[sx][sy][i] = 0;
    queue<T> q;
    q.emplace(sx, sy, 0);
    while(!q.empty()){
        auto [x, y, k] = q.front(); q.pop();
        rep(t, 4){
            int nx = x+dx[t], ny = y+dy[t];
            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            int nk = k;
            if (g[nx][ny] == '#') nk++;
            rep2(s, nk, 3){
                if (dist[nx][ny][s] > dist[x][y][k]+1){
                    dist[nx][ny][s] = dist[x][y][k]+1;
                    q.emplace(nx, ny, s);
                }
            }
        }
    }
    rep(i, 3){
        if (dist[gx][gy][i] != INF){
            cout << "YES" << endl;
            return 0;
        }
    }
    cout << "NO" << endl;
    return 0;
}