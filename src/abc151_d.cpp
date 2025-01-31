#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < n; ++i)
#define rep2(i, a, b) for (ll i = a; i < b; ++i)
#define rep3(i, a, b, c) for (ll i = a; i < b; i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
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

    int ans = -INF;
    rep(sx, h)rep(sy, w)rep(gx, h)rep(gy, w){
        if(sx == gx && sy == gy) continue;
        if(g[sx][sy] == '#' || g[gx][gy] == '#') continue;
        queue<pii> q;
        q.emplace(sx, sy);
        vector<vector<int>> dist(h, vector<int>(w, INF));
        dist[sx][sy] = 0;
        while(!q.empty()){
            auto [x, y] = q.front(); q.pop();
            rep(i, 4){
                int nx = x + dx[i];
                int ny = y + dy[i];
                if(nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
                if(g[nx][ny] == '#') continue;
                if (dist[nx][ny] > dist[x][y] + 1){
                    dist[nx][ny] = dist[x][y] + 1;
                    q.emplace(nx, ny);
                }
            }
        }
        chmax(ans, dist[gx][gy]);
    }
    cout << ans << endl;
    return 0;
}