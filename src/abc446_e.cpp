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
    int m, a, b; cin >> m >> a >> b;
    vector<vector<int>> g(m*m);
    {
        vector<vector<bool>> used(m, vector<bool>(m, false));
        rep(i, m)rep(j, m)
        {
            if (used[i][j]) continue;
            auto dfs = [&](auto f, int s1, int s2) -> void
            {
                used[s1][s2] = true;
                int s3 = (s1*b+s2*a)%m;
                g[s2*m+s3].emplace_back(s1*m+s2);
                if (!used[s2][s3]) f(f, s2, s3);
            };
            dfs(dfs, i, j);
        }
    }
    // rep(i, m)rep(j, m)
    // {
    //     for (int ns: g[i*m+j])
    //     {
    //         int ns1 = ns/m, ns2 = ns%m;
    //         cout << i << " " << j << " -> " << ns1 << " " << ns2 << endl;
    //     }
    // }
    vector<vector<bool>> dist(m, vector<bool>(m, false));
    {
        queue<pii> q;
        rep(i, m)
        {
            q.emplace(i, 0);
            q.emplace(0, i);
            dist[i][0] = true;
            dist[0][i] = true;
        }
        while(q.size())
        {
            auto [s1, s2] = q.front(); q.pop();
            for (int ns: g[s1*m+s2])
            {
                int ns1 = ns/m, ns2 = ns%m;
                if (dist[ns1][ns2]) continue;
                dist[ns1][ns2] = true;
                q.emplace(ns1, ns2);
            }
        }
    }

    int ans = m*m;
    rep(i, m)rep(j, m)
    {
        if (dist[i][j])
        {
            // cout << i << " " << j << endl;
            ans--;
        }
    }
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