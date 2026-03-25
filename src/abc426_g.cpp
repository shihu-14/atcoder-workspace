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
using T4 = tuple<int, int, int, int>;
const int D = 500;
void solve(){
    int n; cin >> n;
    vector<ll> w(n), v(n);
    rep(i, n) cin >> w[i] >> v[i];
    int q; cin >> q;
    vector<T4> query;
    rep(i, q)
    {
        int l, r, c; cin >> l >> r >> c;
        l--;
        query.emplace_back(l, r, c, i);
    }
    vector<vector<ll>> dp(n+1, vector<ll>(D+1, 0));
    vector<ll> ans(q);
    auto dfs = [&](auto f, int l, int r, vector<T4> qs) -> void
    {
        int m = (l+r)/2;
        dp[m] = vector<ll>(D+1, 0);
        for (int i = m-1; i >= l; i--)
        {
            dp[i] = dp[i+1];
            for (int j = D-w[i]; j >= 0; j--)
            {
                chmax(dp[i][j+w[i]], dp[i][j]+v[i]);
            }
        }
        for (int i = m; i < r; i++)
        {
            dp[i+1] = dp[i];
            for (int j = D-w[i]; j >= 0; j--)
            {
                chmax(dp[i+1][j+w[i]], dp[i][j]+v[i]);
            }
        }
        vector<T4> ql, qr;
        for (auto [nl, nr, nc, ni]: qs)
        {
            if (nr < m) ql.emplace_back(nl, nr, nc, ni);
            else if (nl > m) qr.emplace_back(nl, nr, nc, ni);
            else
            {
                ll res = 0;
                rep(i, nc+1)
                {
                    chmax(res, dp[nl][i]+dp[nr][nc-i]);
                }
                ans[ni] = res;
            }
        }
        if (ql.size()) f(f, l, m, ql);
        if (qr.size()) f(f, m, r, qr);
    };
    dfs(dfs, 0, n, query);
    rep(i, q) cout << ans[i] << endl;
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