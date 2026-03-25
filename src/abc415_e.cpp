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

void solve(){
    int h, w; cin >> h >> w;
    vector<vector<ll>> a(h, vector<ll>(w));
    rep(i, h) rep(j, w) cin >> a[i][j];
    vector<ll>  p(h+w-1);
    rep(i, h+w-1) cin >> p[i];
    {
        vector<vector<ll>> dist(h, vector<ll>(w, LINF));
        dist[0][0] = 0;
        queue<pii>  q;
        q.emplace(0, 0);
        while(!q.empty())
        {
            auto [x, y] = q.front(); q.pop();
            a[x][y] -= p[dist[x][y]];
            if (x+1 < h && chmin(dist[x+1][y], dist[x][y]+1))
            {
                dist[x+1][y] = dist[x][y]+1;
                q.emplace(x+1, y);
            }
            if (y+1 < w && chmin(dist[x][y+1], dist[x][y]+1))
            {
                dist[x][y+1] = dist[x][y]+1;
                q.emplace(x, y+1);
            }
        }
    }
    ll wa = -1, ac = LINF;
    while(ac - wa > 1)
    {
        auto f = [&](ll x)
        {
            if (x + a[0][0] < 0) return false;
            vector<vector<ll>> dp(h, vector<ll>(w, -LINF));
            dp[0][0] = x+a[0][0];
            rep(i, h)rep(j, w)
            {
                ll now = dp[i][j];
                if (i+1 < h && now + a[i+1][j] >= 0)
                {
                    chmax(dp[i+1][j], now+a[i+1][j]);
                }
                if (j+1 < w && now + a[i][j+1] >= 0)
                {
                    chmax(dp[i][j+1], now+a[i][j+1]);
                }
            }
            return dp[h-1][w-1] >= 0;
        };
        ll wj = (wa + ac)/2;
        if (f(wj))
        {
            ac = wj;
        }
        else
        {
            wa = wj;            
        }
    }
    cout << ac << endl;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--){
        solve();
    }
    return 0;
}