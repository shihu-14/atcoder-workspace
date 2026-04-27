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

struct UnionFind {
    vector<int> d;
    vector<vector<int>> pos;
    vector<int> col, ws, bs;
    int ans;
    UnionFind(int n=0): d(n,-1), pos(n), col(n), ws(n,1), bs(n), ans(0) {
        rep(i, n) pos[i].emplace_back(i);
    }
    int find(int x) {
        if(d[x] < 0) return x;
        return d[x] = find(d[x]);
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    int merge(int x, int y) { // if merge successed, return true
        int lx = find(x);
        int ly = find(y);
        if (lx == ly)
        {
            if (col[x] == col[y])
            {
                return -1;
            }
            return ans;
        }
        if (d[lx] > d[ly]) 
        {
            swap(lx, ly);
            swap(x, y);
        }
        ans -= min(ws[lx], bs[lx]);
        ans -= min(ws[ly], bs[ly]);
        if (col[x] == col[y])
        {
            swap(ws[ly], bs[ly]);
            for (int z: pos[ly]) 
            {
                pos[lx].emplace_back(z);
                col[z] ^= 1;
            }
        }
        else
        {
            for (int z: pos[ly])
            {
                pos[lx].emplace_back(z);
            }
        }
        ws[lx] += ws[ly];
        bs[lx] += bs[ly];
        ws[ly] = 0;
        bs[ly] = 0;
        pos[ly].clear();
        d[lx] += d[ly];
        d[ly] = lx;
        ans += min(ws[lx], bs[lx]);
        return ans;
    }
    int size(int x){
        return -d[find(x)];
    }
};

void solve()
{
    int n; cin >> n;
    vector<T3> query;
    rep2(i, 1, n+1) 
    {
        int w; cin >> w;
        query.emplace_back(w, 0, i);
    }
    rep2(i, 1, n+1)
    {
        int w; cin >> w;
        if (i < n) query.emplace_back(w, i, i+1);
        else query.emplace_back(w, i, 1);
    }
    sort(rrng(query));
    UnionFind uf(n+1);
    ll ans = 0;
    for (auto [w, u, v] : query)
    {
        int res = uf.merge(u, v);
        if (res == -1)
        {
            ans += w;
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