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
using namespace std;
using namespace atcoder;
using mint = modint998244353;
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

struct Rerooting {
  struct DP {
    int d, id;                 
    DP(): d(-1), id(-1) {}
    DP(int _d, int _id): d(_d), id(_id) {}
    DP operator+(const DP& a) const {
        if (d != a.d) return (d > a.d) ? *this : a;
        return (id > a.id) ? *this : a;
    }
    DP addRoot(int v) const {
        return (d == -1) ? DP(0, v) : DP(d+1, id);
    }
  };
  
  int n;
  vector<vector<int>> to;
  vector<vector<DP>> dp;
  vector<DP> ans;
  Rerooting(int n=0):n(n),to(n),dp(n),ans(n) {}
  void addEdge(int a, int b) {
    to[a].push_back(b);
    to[b].push_back(a);
  }
  void init() {
    dfs(0);
    bfs(0);
  }

  DP dfs(int v, int p=-1) {
    DP dpSum;
    dp[v] = vector<DP>(to[v].size());
    rep(i,to[v].size()) {
      int u = to[v][i];
      if (u == p) continue;
      dp[v][i] = dfs(u,v);
      dpSum = dpSum + dp[v][i];
    }
    return dpSum.addRoot(v);
  }
  void bfs(int v, const DP& dpP=DP(), int p=-1) {
    int deg = to[v].size();
    rep(i,deg) if (to[v][i] == p) dp[v][i] = dpP;

    vector<DP> dpSumL(deg+1);
    rep(i,deg) dpSumL[i+1] = dpSumL[i] + dp[v][i];
    vector<DP> dpSumR(deg+1);
    for (int i = deg-1; i >= 0; --i)
      dpSumR[i] = dpSumR[i+1] + dp[v][i];

    ans[v] = dpSumL[deg].addRoot(v);

    rep(i,deg) {
      int u = to[v][i];
      if (u == p) continue;
      DP d = dpSumL[i] + dpSumR[i+1];
      bfs(u, d.addRoot(v), v);
    }
  }
};

void solve()
{
    int n; cin >> n;
    Rerooting g(n);
    rep(i, n-1)
    {
        int a, b; cin >> a >> b; a--; b--;
        g.addEdge(a, b);
    }
    g.init();
    rep(i, n) cout << g.ans[i].id + 1 << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    while(t--)
    {
        solve();
    }
    return 0;
}
