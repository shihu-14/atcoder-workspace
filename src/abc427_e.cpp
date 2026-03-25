#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cwctype>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <tuple>
#include <array>
#include <new>
#include <memory>
#include <limits>
#include <random>
#include <exception>
#include <stdexcept>
#include <regex>
#include <complex>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
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
template<class T>
using G = vector<vector<T>>;
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
int dp[15][15][15][15][30][30];
using T6 = tuple<int, int, int, int, int, int>;

struct CumSum {
    int h, w; 
    vector<vector<ll>> s;
    CumSum(vector<vector<ll>> &a): h(a.size()), w(a[0].size()){
        s.resize(h+1, vector<ll>(w+1));
        rep(i, h)rep(j, w) s[i+1][j+1] = s[i+1][j] + a[i][j];
        rep(j, w+1)rep(i, h) s[i+1][j] += s[i][j];
    }
    ll query(int lx, int ly, int rx, int ry){ // [ly, ry), [lx, rx)の総和
        if (lx < 0 || ly < 0 || rx > h || ry > w) return 0;
        return s[rx][ry] - s[lx][ry] - s[rx][ly] + s[lx][ly];
    }
};

void solve()
{
    int h, w; cin >> h >> w;
    vector<vector<ll>> g(h, vector<ll>(w));
    int sx, sy;
    rep(i, h)rep(j, w)
    {
        char c; cin >> c;
        if (c == '#') g[i][j] = 1;
        else if (c == 'T') sx = i, sy = j;
    }
    CumSum cs(g);
    rep(lx, 15) rep(ly, 15) rep(rx, 15) rep(ry, 15) rep(xx, 30) rep(yy, 30) dp[lx][ly][rx][ry][xx][yy] = INF;
    dp[0][0][h][w][h][w] = 0;
    queue<T6> q;
    q.emplace(0, 0, h, w, h, w);
    while(!q.empty())
    {
        auto [lx, ly, rx, ry, xx, yy] = q.front(); q.pop();
        int d = cs.query(lx, ly, rx, ry);
        if (d == 0)
        {
            cout << dp[lx][ly][rx][ry][xx][yy] << endl;
            return;
        }
        rep(t, 4)
        {
            int nlx = lx, nly = ly, nrx = rx, nry = ry, nxx = xx, nyy = yy;
            nxx += dx[t];
            nyy += dy[t];
            chmax(nlx, h-nxx);
            chmax(nly, w-nyy);
            chmin(nrx, 2*h-nxx);
            chmin(nry, 2*w-nyy);
            if (nxx < 0 || nyy < 0 || nxx > 2*h || nyy > 2*w) continue;
            int gx = sx + h - nxx, gy = sy + w - nyy;
            if (!(nlx <= gx && gx < nrx && nly <= gy && gy < nry && g[gx][gy] == 1))
            {
                if (chmin(dp[nlx][nly][nrx][nry][nxx][nyy], dp[lx][ly][rx][ry][xx][yy] + 1))
                {
                    q.emplace(nlx, nly, nrx, nry, nxx, nyy);
                }
            }
        }
    }
    cout << -1 << endl;
}   

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}