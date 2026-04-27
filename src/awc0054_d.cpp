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
#define rrep2(i, a, b) for (ll i = int(a)-1; i >= (b); --i)
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
    int n; cin >> n;
    vector<ll> a(n), s(n+1);
    rep(i, n) cin >> a[i];
    rep(i, n) s[i+1] = s[i]+a[i];

    vector<vector<vector<pll>>> memo(n, vector<vector<pll>>(n, vector<pll>(2, {-LINF, -LINF})));
    auto f = [&](auto f, int l, int r, int flag) -> pll
    {
        if (r-l < 0) return {0, 0};
        // if (r-l == 0) return a[l];
        if (memo[l][r][flag] != pll{-LINF, -LINF}) return memo[l][r][flag];
        if (flag == 1)
        {
            ll res_t = -LINF, res_a = -LINF;
            auto [lt, la] = f(f, l+1, r, flag^1);
            auto [rt, ra] = f(f, l, r-1, flag^1);
            if (res_t < lt+a[l])
            {
                res_t = lt+a[l];
                res_a = la;
            }
            if (res_t < rt+a[r])
            {
                res_t = rt+a[r];
                res_a = ra;
            }
            return memo[l][r][flag] = {res_t, res_a};
        }
        else
        {
            ll res_t = -LINF, res_a = -LINF;
            auto [lt, la] = f(f, l+1, r, flag^1);
            auto [rt, ra] = f(f, l, r-1, flag^1);
            if (res_a < la+a[l])
            {
                res_t = lt;
                res_a = la+a[l];
            }
            if (res_a < ra+a[r])
            {
                res_t = rt;
                res_a = ra+a[r];
            }
            return memo[l][r][flag] = {res_t, res_a};
        }
    };

    auto [ans_t, ans_a] = f(f, 0, n-1, 1);
    cout << ans_t << " " << ans_a << endl;
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