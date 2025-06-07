// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
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

const int M = 12;
const int A = 6;
const ll  L = 1'000'000;

struct P {
    string s;
    int w, l;
    array<int, A> c{};
};

double es(const array<double, A>& p, const vector<P>& v) {
    double r = 0;
    for (const auto& t : v) {
        double lg = 0;
        rep(i, A) if (t.c[i]) lg += t.c[i] * log(p[i]);
        double pr = exp(lg);
        if (pr <= 0) continue;
        double lam = (L - t.l + 1) * pr;
        r += t.w * (1 - exp(-lam));
    }
    return r;
}

void bm(const array<double, A>& p, array<array<int, M>, M>& a) {
    array<double, M> raw;
    rep(i, M) raw[i] = p[i % A] / 2;
    array<int, M> row;
    array<pair<double, int>, M> fr;
    int s = 0;
    rep(i, M) {
        double x = raw[i] * 100;
        int v = floor(x);
        row[i] = v; s += v;
        fr[i] = {x - v, i};
    }
    int r = 100 - s;
    sort(rng(fr), [](auto x, auto y) { return x.first > y.first; });
    rep(i, r) row[fr[i].second]++;
    rep(i, M) rep(j, M) a[i][j] = row[j];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d1, d2;
    if (!(cin >> n >> d1 >> d2)) return 0;

    vector<P> v(n);
    rep(i, n) {
        cin >> v[i].s >> v[i].w;
        v[i].l = v[i].s.size();
        for (char ch : v[i].s) v[i].c[ch - 'a']++;
    }

    array<double, A> bp;
    rep(i, A) bp[i] = 1.0 / A;
    double bs = es(bp, v);

    mt19937_64 rg(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> ur(0, 1);
    auto st = chrono::steady_clock::now();

    while (chrono::duration<double>(chrono::steady_clock::now() - st).count() < 1.9) {
        auto cp = bp;
        int x = rg() % A, y;
        do y = rg() % A; while (y == x);
        double d = (ur(rg) - 0.5) * 0.1;
        double t = min(cp[x], max(0.0, d));
        double g = min(cp[y], max(0.0, -d));
        cp[x] -= t; cp[y] += t; cp[x] += g; cp[y] -= g;
        double sm = accumulate(rng(cp), 0.0);
        rep(i, A) cp[i] = max(1e-6, cp[i] / sm);
        double sc = es(cp, v);
        if (sc > bs) { bs = sc; bp = cp; }
    }

    array<array<int, M>, M> a{};
    bm(bp, a);

    rep(i, M) {
        cout << char('a' + i % A);
        rep(j, M) cout << ' ' << a[i][j];
        cout << '\n';
    }
    return 0;
}
