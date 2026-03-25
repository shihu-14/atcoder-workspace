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
const ll LINF = 4001002003004005006ll;
const int INF = 1001001001;

void solve()
{
    int n; ll c; cin >> n >> c;
    vector<vector<pll>> g(3);
    rep(i, n)
    {
        ll w, v, k; cin >> w >> v >> k; w--;
        g[w].emplace_back(v, k);
    }
    rep(i, 3) sort(rrng(g[i])); 
    vector<vector<ll>> sum_k(3), sum_v(3);
    rep(i, 3)
    {
        ll tot = 0;
        rep(j, g[i].size())
        {
            tot += g[i][j].se;
            sum_k[i].emplace_back(g[i][j].se);
            sum_v[i].emplace_back(g[i][j].fi*g[i][j].se);
            if (tot > c) 
            {
                break;
            }
        }
        rep(j, (int)sum_k[i].size()-1)
        {
            sum_k[i][j+1] += sum_k[i][j];
            sum_v[i][j+1] += sum_v[i][j];
        }
    }
    auto sum = [&](ll w, ll k) -> ll
    {
        if (g[w].empty()) return 0;
        if (sum_k[w].back() <= k) return sum_v[w].back();

        ll res = 0;
        int id = lower_bound(rng(sum_k[w]), k) - sum_k[w].begin();
        if (id) 
        {
            res += sum_v[w][id-1];
            ll rem = k-sum_k[w][id-1];
            res += rem * g[w][id].fi;
        }
        else
        {
            res += k * g[w][id].fi;
        }
        return res;
    };

    auto f = [&](ll x) -> ll
    {
        if (c-3*x < 0) return -LINF;
        ll l = 0, r = min((c-3*x)/2, (sum_k[1].empty() ? 0: sum_k[1].back()))+1;
        auto f2 = [&](ll y) -> ll
        {
            if (c - 3*x - 2*y < 0) return -LINF;
            return sum(2, x) + sum(1, y) + sum(0, c - 3*x - 2*y);
        };
        while(r-l > 2)
        {
            ll m1 = l + (r - l) / 3;
            ll m2 = r - (r - l) / 3;
            if (f2(m1) < f2(m2)) l = m1;
            else r = m2;
        }
        ll res = 0;
        // rep2(i, l, r) chmax(res, f2(i));
        for (ll i = max(0LL, l-1); i <= r; i++) chmax(res, f2(i));
        return res;
    };
    
    ll l = 0, r = min(c/3, (sum_k[2].empty() ? 0: sum_k[2].back()))+1;
    while(r-l > 2)
    {
        ll m1 = l + (r - l) / 3;
        ll m2 = r - (r - l) / 3;
        if (f(m1) < f(m2)) l = m1;
        else r = m2;
    }

    ll ans = 0;
    // rep2(i, l, r+1) chmax(ans, f(i));
    for (ll i = max(0LL, l-1); i <= r; i++) chmax(ans, f(i));
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