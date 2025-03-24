// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
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
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    int sx, sy;
    {
        rep(i, h)rep(j, w){
            if (g[i][j] == 'S'){
                sx = i;
                sy = j;
            }
        }
    }

    rep(t, 4){
        int tx = sx+dx[t], ty = sy+dy[t];
        if (tx < 0 || tx >= h || ty < 0 || ty >= w) continue;
        if (g[tx][ty] == '#') continue;
        queue<pii> q; q.emplace(tx, ty);
        vector<vector<int>> dist(h, vector<int>(w, INF));
        dist[tx][ty] = 0;
        while(!q.empty()){
            auto [x, y] = q.front(); q.pop();
            rep(i, 4){
                int nx = x+dx[i], ny = y+dy[i];
                if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
                if (g[nx][ny] != '.') continue;
                if (dist[nx][ny] != INF) continue;
                dist[nx][ny] = dist[x][y]+1;
                q.emplace(nx, ny);
            }
        }
        bool flag = false;
        rep(tt, 4){
            int ttx = sx+dx[tt], tty = sy+dy[tt];
            if (ttx < 0 || ttx >= h || tty < 0 || tty >= w) continue;
            if (g[ttx][tty] == '#') continue;
            if (ttx == tx && tty == ty) continue;
            if (dist[ttx][tty] != INF){
                flag = true;
                break;
            }
        }
        if (flag){
            cout << "Yes" << endl;
            return 0;
        }
    }
    cout << "No" << endl;
    return 0;
}