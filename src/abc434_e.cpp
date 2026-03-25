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
#define MAx(v) *max_element(rng(v))
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

template<typename T=int>
struct CC {
    bool initialized;
    vector<T> xs;
    CC(): initialized(false) {}
    void add(T x) { xs.push_back(x);}
    void init() {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(),xs.end()),xs.end());
        initialized = true;
    }
    // x以下の登録されている値のうち最大のもののインデックスを返す. 見つからなければ-1を返す.
    int operator()(T x) {
        if (!initialized) init();
        return upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
    }
    T operator[](int i) {
        if (!initialized) init();
        return xs[i];
    }
    int size() {
        if (!initialized) init();
        return xs.size();
    }
};

void solve()
{
    int n; cin >> n;
    vector<ll> x(n), r(n);
    CC ccl, ccr;
    rep(i, n)
    {
        cin >> x[i] >> r[i];
        ccr.add(x[i]+r[i]);
        ccr.add(x[i]-r[i]);
    }
    int R = ccr.size();
    int s = n+R, t = s+1;
    mf_graph<int> g(t+1);
    rep(i, n)
    {
        g.add_edge(i, n+ccr(x[i]+r[i]), INF);
        g.add_edge(i, n+ccr(x[i]-r[i]), INF);
    }
    rep(i, n)
    {
        g.add_edge(s, i, 1);
    }
    rep(i, R)
    {
        g.add_edge(n+i, t, 1);
    }
    cout << g.flow(s, t) << endl;
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
//     int n; cin >> n;
//     vector<ll> x(n), R(n);
//     CC cc;
//     rep(i, n)
//     {
//         cin >> x[i] >> r[i];
//         cc.add(x[i]+r[i]);
//         cc.add(x[i]-r[i]);
//     }
//     int m = cc.size();
//     vector<int> c(m);
//     vector<bool> used(m);
//     vector<set<int>> pos(m);
//     rep(i, n)
//     {
//         int c1 = cc(x[i]-r[i]);
//         int c2 = cc(x[i]+r[i]);
//         pos[c1].emplace(i);
//         pos[c2].emplace(i);
//         c[c1]++;
//         c[c2]++;
//     }
//     set<pii> s;
//     rep(i, m) if (c[i]) s.emplace(c[i], i);
//     while(s.size())
//     {
//         auto [cnt, x] = *s.begin();
//         int id = *pos[x].begin();
//         used[x] = true;
//         s.erase(s.begin());
//         c[x] = 0;
//         int y = (x == cc(x[id]-r[id]) ? cc(x[id]+r[id]) : cc(x[id]-r[id]));
//         pos[y].erase(id);
//         if (c[y]) c[y]--;
//         s.erase({c[y]+1, y});
//         if (c[y]) s.emplace(c[y], y);
//     }

//     int ans = 0;
//     rep(i, m) if (used[i]) ans++;
//     cout << ans << endl;
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