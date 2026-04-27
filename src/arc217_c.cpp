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
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j].val()<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

void solve()
{
    ll n, c; cin >> n >> c; 
    vector<ll> a(n);
    rep(i, n) cin >> a[i];
    sort(rng(a));
    vector<mint> fact(n+1, 1), fact_inv(n+1);
    rep(i, n) fact[i+1] = fact[i]*(i+1);
    rep(i, n+1) fact_inv[i] = fact[i].inv();

    vector<vector<mint>> p(n+1, vector<mint>(n+1, 1));
    rep(i, n+1)
    {  
        if (i == 0) p[1][i] = (mint)(a[0]-1)/c;
        else if (i == n) p[1][i] = (mint)(c+1-a[n-1])/c;
        else p[1][i] = (mint)(a[i]-a[i-1])/c;
    }
    rep2(i, 1, n)
    {
        rep(j, n+1)
        {
            p[i+1][j] = p[i][j]*p[1][j];
        }
    }
    vector<vector<mint>> dp(n+1, vector<mint>(n+1));
    rep(i, n+1) dp[0][i] = p[i][0]*fact_inv[i];
    rep2(i, 1, n+1)
    {
        vector<vector<mint>> old(n+1, vector<mint>(n+1)); swap(old, dp);
        rep(j, i)
        {
            rep(k, n+1)
            {
                if (old[j][k] == 0) continue;
                rep(nk, n-k+1)
                {
                    dp[j+min(nk, i-j)][k+nk] += old[j][k]*p[nk][i]*fact_inv[nk];
                }
            }
        }
    }
    rep(i, n+1)
    {
        mint ans = dp[i][n]*fact[n];
        cout << ans.val() << " ";
    }
    cout << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}