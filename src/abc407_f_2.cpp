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

// 構造体 version
template <typename T>
struct CartesianTree {
    int n, root;
    vector<int> l, r, p;
    CartesianTree() {}
    CartesianTree(const vector<T>& a) : n(a.size()), l(n, -1), r(n, -1), p(n, -1), root(-1) {
        vector<int> stk;
        for (int i = 0; i < n; ++i) {
            int last = -1;
            while (!stk.empty() && a[i] > a[stk.back()]) { // max: a[i] > a[stk.back()]
                last = stk.back();
                stk.pop_back();
            }
            if (last != -1) {
                p[last] = i;
                l[i] = last;
            }
            if (!stk.empty()) {
                p[i] = stk.back();
                r[stk.back()] = i;
            }
            stk.emplace_back(i);
        }
        for (int i = 0; i < n; ++i) {
            if (p[i] == -1) {
                root = i;
                break;
            }
        }
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<ll> a(n);
    rep(i, n) cin >> a[i];
    CartesianTree g(a);
    vector<ll> s0(n+3), s1(n+3);
    auto dfs = [&](auto f, int v) -> int{
        if (v == -1) return 0;
        int l = f(f, g.l[v])+1;
        int r = f(f, g.r[v])+1;
        if (l > r) swap(l, r);
        s1[1] += a[v];
        s1[l+1] -= a[v];
        
        s0[l+1] += a[v]*l;
        s0[r+1] -= a[v]*l;

        s1[r+1] -= a[v];
        s1[l+r+1] += a[v];

        s0[r+1] += a[v]*(l+r);
        s0[l+r+1] -= a[v]*(l+r);

        return l+r-1;
    };
    dfs(dfs, g.root);
    rep(i, n+2) s0[i+1] += s0[i];
    rep(i, n+2) s1[i+1] += s1[i];
    rep2(i, 1, n+1) cout << s0[i]+s1[i]*i << endl;
    return 0;
}