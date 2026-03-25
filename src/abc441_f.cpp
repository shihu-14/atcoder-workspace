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
    int n, m; cin >> n >> m;
    vector<ll> p(n), v(n);
    rep(i, n) cin >> p[i] >> v[i];
    vector<vector<ll>> dp(n+1, vector<ll>(m+1, -LINF));
    dp[0][0] = 0;
    rep(i, n)
    {
        rep(j, m+1)
        {
            if (dp[i][j] == -LINF) continue;
            chmax(dp[i+1][j], dp[i][j]);
            if (j+p[i] <= m)
            {
                chmax(dp[i+1][j+p[i]], dp[i][j] + v[i]);
            }
        }
    }
    ll max_v = -LINF;
    rep(j, m+1) chmax(max_v, dp[n][j]);

    string ans;
    vector<ll> dp2(m+1, -LINF);
    dp2[0] = 0;
    rrep2(i, n, 0)
    { 
        vector<ll> old(m+1, -LINF); swap(old, dp2);
        rep(j, m) chmax(dp[i][j+1], dp[i][j]);

        ll max_a = -LINF, max_c = -LINF; 
        rep(j, m+1)
        {
            if (old[j] == -LINF) continue;
            if (m-j-p[i] >= 0)
            {
               chmax(max_a, old[j]+v[i]+dp[i][m-j-p[i]]);
            }
            chmax(max_c, old[j]+dp[i][m-j]);
        }
        if (max_a == max_v && max_c == max_v)
        {
            ans += 'B';
        }
        else if (max_a == max_v)
        {
            ans += 'A';
        }
        else
        {
            ans += 'C';
        }
        rep(j, m+1)
        {
            if (old[j] == -LINF) continue;
            chmax(dp2[j], old[j]);
            if (j+p[i] <= m)
            {
                chmax(dp2[j+p[i]], old[j]+v[i]);
            }
        }
    }
    reverse(rng(ans));
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