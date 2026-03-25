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
using T4 = tuple<int, int, int, int>;
void solve()
{
    int h, w; cin >> h >> w;
    vector<string> s(h);
    rep(i, h) cin >> s[i];
    vector<vector<vector<vector<T4>>>> g(h, vector<vector<vector<T4>>>(w, vector<vector<T4>>(4)));
    rep(i, h)rep(j, w)
    {
        rep(d, 4)
        {
            int nd = (d+2)%4;
            int ni = i+dx[d];
            int nj = j+dy[d];
            if (ni < 0 || ni >= h || nj < 0 || nj >= w) continue;
            g[i][j][d].emplace_back(ni, nj, nd, 0);
        }
        rep(d, 4)
        {
            if (d%2 == 0)
            {
                g[i][j][d].emplace_back(i, j, (d+2)%4, s[i][j] != 'A');
                g[i][j][d].emplace_back(i, j, (d+1)%4, s[i][j] != 'B');
                g[i][j][d].emplace_back(i, j, (d+3)%4, s[i][j] != 'C');
            }
            else
            {
                g[i][j][d].emplace_back(i, j, (d+2)%4, s[i][j] != 'A');
                g[i][j][d].emplace_back(i, j, (d+1)%4, s[i][j] != 'C');
                g[i][j][d].emplace_back(i, j, (d+3)%4, s[i][j] != 'B');
            }
        }
    }
    deque<T3> q;
    q.emplace_back(0, 0, 3);
    vector<vector<vector<int>>> dist(h, vector<vector<int>>(w, vector<int>(4, INF)));
    dist[0][0][3] = 0;
    while(!q.empty())
    {
        auto [i, j, d] = q.front(); q.pop_front();
        for (auto [ni, nj, nd, cost] : g[i][j][d])
        {
            if (chmin(dist[ni][nj][nd], dist[i][j][d] + cost))
            {
                if (cost == 0) q.emplace_front(ni, nj, nd);
                else q.emplace_back(ni, nj, nd);
            }
        }
    } 
    cout << dist[h-1][w-1][1] << '\n';
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