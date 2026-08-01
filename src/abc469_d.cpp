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
    int n, m; cin >> n >> m;
    int v1, v2;
    int all_edges = 0;
    vector<set<int>> g(n);
    rep(i, m)
    {
        cin >> v1 >> v2; v1--; v2--;
        if (g[v1].count(v2)) continue;
        all_edges++;
        g[v1].emplace(v2);
        g[v2].emplace(v1);
    }
    set<pii> ans;
    for (ll v: {v1, v2})
    {
        rep(u, n)
        {
            if (u == v) continue;
            int cnt = g[u].size() + g[v].size();
            if (g[u].count(v)) cnt--;
            if (cnt == all_edges)
            {
                ans.emplace(min(u, v), max(u, v));
            }
        }
    }
    cout << ans.size() << endl;
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

// void solve()
// {
//     int n, m; cin >> n >> m;
//     vector<int> deg(n);
//     vector<vector<int>> g(n);
//     set<pii> edges;
//     rep(i, m)
//     {
//         int a, b; cin >> a >> b; a--; b--;
//         if (edges.count(pii(a, b))) continue;
//         edges.emplace(a, b);
//         g[a].emplace_back(b);
//         g[b].emplace_back(a);
//         deg[a]++;
//         deg[b]++;
//     }
//     int cnt = 0;
//     rep(i, n) if (deg[i] >= 3) cnt++;
//     if (cnt >= 3)
//     {
//         cout << 0 << endl;
//         return;
//     }
//     if (cnt == 2)
//     {
//         int v1 = -1, v2 = -1;
//         rep(i, n)
//         {
//             if (deg[i] >= 3)
//             {
//                 if (v1 == -1) v1 = i;
//                 else v2 = i;
//             }
//         }
//         set<pii> used_edges = edges;
//         for (int u: g[v1]) used_edges.erase({min(u, v1), max(u, v1)});
//         for (int u: g[v2]) used_edges.erase({min(u, v2), max(u, v2)});
//         if (used_edges.empty())
//         {
//             cout << 1 << endl;
//         }
//         else
//         {
//             cout << 0 << endl;
//         }
//     }
//     else if (cnt == 1)
//     {
//         int v = -1;
//         rep(i, n) if (deg[i] >= 3) v = i;
//         set<pii> used_edges = edges;
//         for (int u: g[v]) used_edges.erase({min(u, v), max(u, v)});
//         if (used_edges.empty())
//         {
//             cout << n-1 << endl;
//         }
//         else if (used_edges.size() == 1)
//         {
//             cout << 2 << endl;
//         }
//         else
//         {
//             set<int> vs;
//             for (auto [a, b]: used_edges)
//             {
//                 vs.emplace(a);
//                 vs.emplace(b);
//             }
//             if (vs.size() == 3 && used_edges.size() == 2)
//             {
//                 cout << 1 << endl;
//             }
//             else
//             {
//                 cout << 0 << endl;
//             }
//         }
//     }
//     else
//     {
//         if (edges.size() == 1)
//         {
//             cout << 2*(n-1)-1 << endl;
//             return;
//         }
//         vector<int> ds;
//         rep(i, n) if (deg[i]) ds.emplace_back(deg[i]);
//         sort(rng(ds));
//         if (ds == vector<int>{1, 1, 1, 1})
//         {
//             cout << 4 << endl;
//         }
//         else if (ds == vector<int>{1, 1, 1, 1, 2, 2})
//         {
//             cout << 1 << endl;
//         }
//         else if (ds == vector<int>{1, 1, 2, 2})
//         {
//             cout << 3 << endl;
//         }
//         else if (ds == vector<int>{2, 2, 2})
//         {
//             cout << 3 << endl;
//         }
//         else if (ds == vector<int>{2, 2, 2, 2})
//         {
//             cout << 2 << endl;
//         }
//         else if (ds == vector<int>{1, 1, 2})
//         {
//             cout << n << endl;
//         }
//         else
//         {
//             cout << 0 << endl;
//         }
//     }
// }

// int main()
// {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int t = 1;
//     // cin >> t;
//     while(t--)
//     {
//         solve();
//     }
//     return 0;
// }