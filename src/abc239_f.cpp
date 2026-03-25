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
    vector<int> deg(n);
    rep(i, n) cin >> deg[i];
    if (SUM(deg) != 2*(n-1))
    {
        cout << -1 << endl;
        return;
    }

    dsu uf(n);
    rep(i, m)
    {
        int u, v; cin >> u >> v; u--; v--;
        deg[u]--; deg[v]--;
        if (uf.same(u, v))
        {
            cout << -1 << endl;
            return;
        }
        uf.merge(u, v);
    }

    vector<vector<int>> g(n);
    vector<int> ones, others;
    {
        for (auto &vs: uf.groups())
        {
            int sum = 0;
            int id = uf.leader(vs[0]);
            for (auto v: vs)
            {
                if (deg[v] < 0)
                {
                    cout << -1 << endl;
                    return;
                }
                rep(i, deg[v]) g[id].emplace_back(v);
                sum += deg[v];
            }
            if (sum == 0)
            {
                cout << -1 << endl;
                return;
            }
            if (sum == 1) ones.emplace_back(id);
            else others.emplace_back(id);
        }
    }

    vector<pii> ans;
    while(ones.size() && others.size())
    {
        int u = ones.back(); ones.pop_back();
        int v = others.back(); others.pop_back();
        ans.emplace_back(g[u].back(), g[v].back());
        g[u].pop_back();
        g[v].pop_back();
        if (g[v].size() == 1) ones.emplace_back(v);
        else others.emplace_back(v);
    }
    // if (ones.size() != 2 && ones.size() != 0 || others.size() != 0)
    // {
    //     cout << -1 << endl;
    //     return;
    // }
    if (ones.size() == 2)
    {
        int u = ones.back(); ones.pop_back();
        int v = ones.back(); ones.pop_back();
        ans.emplace_back(g[u].back(), g[v].back());
    }
    for (auto [u, v]: ans) cout << u+1 << " " << v+1 << endl;
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