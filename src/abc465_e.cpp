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

mint dp[510][2][3][1200];
void solve()
{
    string n; cin >> n;
    rep(i, 510) rep(j, 2) rep(k, 3) rep(l, 1200) dp[i][j][k][l] = 0;
    dp[0][0][0][0] = 1;
    rep(i, n.size())
    {
        rep(j, 2)
        {
            rep(r, 3)
            {
                rep(s, 1<<10)
                {
                    rep(d, 10)
                    {
                        if (j == 0 && d > n[i]-'0') continue;
                        int nj = j;
                        if (j == 0 && d < n[i]-'0') nj = 1;
                        int nr = (r+d)%3;
                        int ns = s|(1<<d);
                        if (s == 0 && d == 0) ns = 0;
                        dp[i+1][nj][nr][ns] += dp[i][j][r][s];
                    }
                }
            }
        }
    }
    mint ans1, ans2, ans3, ans12, ans13, ans23, ans123;
    {
        rep(j, 2)
        {
            rep2(s, 1, 1<<10)
            {
                ans1 += dp[n.size()][j][0][s];
            }
        }
    }
    {
        rep(j, 2)
        {
            rep(r, 3)
            {
                rep2(s, 1, 1<<10)
                {
                    if (s>>3&1) ans2 += dp[n.size()][j][r][s];
                }
            }
        }
    }
    {
        rep(j, 2)
        {
            rep(r, 3)
            {
                rep2(s, 1, 1<<10)
                {
                    if (popcount(s) == 3) ans3 += dp[n.size()][j][r][s];
                }
            }
        }
    }
    {
        rep(j, 2)
        {
            rep2(s, 1, 1<<10)
            {
                if (s>>3&1) ans12 += dp[n.size()][j][0][s];
            }
        }
    }
    {
        rep(j, 2)
        {
            rep2(s, 1,  1<<10)
            {
                if (popcount(s) == 3) ans13 += dp[n.size()][j][0][s];
            }
        }
    }
    {
        rep(j, 2)
        {
            rep(r, 3)
            {
                rep2(s, 1, 1<<10)
                {
                    if (s>>3&1 && popcount(s) == 3) ans23 += dp[n.size()][j][r][s];
                }
            }
        }
    }
    {
        rep(j, 2)
        {
            rep2(s, 1, 1<<10)
            {
                if (s>>3&1 && popcount(s) == 3) ans123 += dp[n.size()][j][0][s];
            }
        }
    }
    mint ans = ans1 + ans2 + ans3 - 2*(ans12 + ans13 + ans23) + 3*ans123;
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