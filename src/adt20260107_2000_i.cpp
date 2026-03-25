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

void solve()
{
    int h, w; cin >> h >> w;
    vector<int> R(h), C(w);
    rep(i, h) cin >> R[i];
    rep(i, w) cin >> C[i];
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    vector<vector<vector<ll>>> dp(h, vector<vector<ll>>(w, vector<ll>(4, LINF)));
    rep(t, 4)
    {
        ll res = 0;
        res += (t&1) ? C[0] : 0;
        res += (t>>1&1) ? R[0] : 0;
        dp[0][0][t] = res;
    }
    rep(i, h)rep(j, w)
    {
        rep(t, 4)
        {
            ll now = dp[i][j][t];
            if (now == LINF) continue;
            bool col_r = (t&1);
            bool row_r = (t>>1&1);
            int now_a = g[i][j]-'0';
            now_a ^= col_r;
            now_a ^= row_r;
            if (i+1 < h)
            {
                int to_a = g[i+1][j]-'0';
                to_a ^= col_r;
                int nt = t&1 | ((to_a ^ now_a) << 1);
                chmin(dp[i+1][j][nt], now+((nt>>1&1) ? R[i+1] : 0));
            }
            if (j+1 < w)
            {
                int to_a = g[i][j+1]-'0';
                to_a ^= row_r;
                int nt = (to_a ^ now_a) | (t>>1&1)<<1;
                chmin(dp[i][j+1][nt], now+((nt&1) ? C[j+1] : 0));
            }
        }
    }
    ll ans = LINF;
    rep(t, 4) chmin(ans, dp[h-1][w-1][t]);
    cout << ans << endl;
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