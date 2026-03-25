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
    int h, w, n; cin >> h >> w >> n;
    vector<int> hs(n), ws(n);
    rep(i, n) cin >> hs[i] >> ws[i];
    map<int, set<pii>> inv_hs, inv_ws;
    rep(i, n)
    {
        inv_hs[hs[i]].emplace(ws[i], i);
        inv_ws[ws[i]].emplace(hs[i], i);
    }
    // for (auto& [k, v] : inv_hs) sort(rng(v));
    // for (auto& [k, v] : inv_ws) sort(rng(v));

    int now_h = h, now_w = w;
    int now_x = 1, now_y = 1;
    vector<pii> ans(n);
    vector<bool> used(n);
    rep(i, n)
    {
        if (inv_hs[now_h].size())
        {
            auto [max_w, max_h_id] = *inv_hs[now_h].rbegin();
            ans[max_h_id] = {now_x, now_y};
            now_y += max_w;
            now_w -= max_w;
            inv_hs[now_h].erase(prev(inv_hs[now_h].end()));
            inv_ws[max_w].erase({now_h, max_h_id});
        }
        else
        {
            auto [max_h, max_w_id] = *inv_ws[now_w].rbegin();
            ans[max_w_id] = {now_x, now_y};
            now_x += max_h;
            now_h -= max_h;
            inv_ws[now_w].erase(prev(inv_ws[now_w].end()));
            inv_hs[max_h].erase({now_w, max_w_id});
        }
    }
    for (auto [i, j]: ans)
    {
        cout << i << " " << j << endl;
    }
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

