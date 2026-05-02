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
    vector<set<int>> g(n);
    
    rep(i, m)
    {
        int u, v; cin >> u >> v; u--, v--;
        g[u].emplace(v);
        g[v].emplace(u);
    } 
    int w; cin >> w;
    vector<string> s(n);
    rep(i, n) cin >> s[i];
    scc_graph sg(w*n);
    vector<vector<int>> g2(n*w);
    rep(v, n)
    {
        rep(vi, w)
        {
            int ui = (vi + 1) % w;
            if (s[v][vi] == 'o' && s[v][ui] == 'o')
            {
                sg.add_edge(v*w+vi, v*w+ui);
                g2[v*w+vi].emplace_back(v*w+ui);
            }
        }
        for (int u : g[v])
        {
            rep(vi, w)
            {
                int ui = (vi + 1) % w;
                if (s[v][vi] == 'o' && s[u][ui] == 'o')
                {
                    sg.add_edge(v*w+vi, u*w+ui);
                    g2[v*w+vi].emplace_back(u*w+ui);
                }
            }
        }
    }
    auto scc = sg.scc();
    vector<vector<bool>> used(n, vector<bool>(w)), used2(n, vector<bool>(w));
    for (auto vs: scc)
    {
        if (vs.size() == 1) continue;
        for (int x : vs)
        {
            used[x/w][x%w] = true;
        }
    }
    // printvv(used);
    queue<int> q;
    rep(i, n)
    {
        if (s[i][0] == 'o')
        {
            q.emplace(i*w);
            used2[i][0] = true;
            // cout << i << " ";
        }
    }
    // cout << endl;
    while(q.size())
    {
        int v = q.front(); q.pop();
        if (used[v/w][v%w])
        {
            cout << "Yes" << endl;
            return;
        }
        for (int u : g2[v])
        {
            if (used2[u/w][u%w]) continue;
            used2[u/w][u%w] = true;
            q.emplace(u);
        }
    }
    cout << "No" << endl;
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