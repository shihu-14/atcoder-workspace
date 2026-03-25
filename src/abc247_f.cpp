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
    vector<int> p(n), q(n);
    rep(i, n) cin >> p[i], p[i]--;
    rep(i, n) cin >> q[i], q[i]--;
    vector<int> g(n);
    rep(i, n) g[p[i]] = q[i];
    mint ans = 1;
    {
        vector<bool> used(n);
        rep(i, n)
        {
            if (used[i]) continue;
            vector<int> vs;
            auto dfs = [&](auto f, int u) -> void
            {
                used[u] = true;
                vs.emplace_back(u);
                int v = g[u];
                if (!used[v])
                {
                    f(f, v);
                }
            };
            dfs(dfs, i);
            int m = vs.size();
            if (m <= 1) continue;
            // cout << m << endl;
            // printv(vs);
            vector<vector<mint>> dp(2, vector<mint>(2));
            dp[0][0] = dp[1][1] = 1;
            rep(i, m-1)
            {
                vector<vector<mint>> old(2, vector<mint>(2)); swap(old, dp);
                rep(j, 2)rep(k, 2)
                {
                    if (i == m-2)
                    {
                        if (j == 1 && k == 1) dp[0][k] += old[j][k];
                        dp[1][k] += old[j][k];
                    }
                    else
                    {   
                        if (j == 1) dp[0][k] += old[j][k];
                        dp[1][k] += old[j][k];
                    }
                }
            }
            mint res = 0;
            rep(j, 2)rep(k, 2) res += dp[j][k];
            ans *= res;
        }
    }
    cout << ans.val() << endl;
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