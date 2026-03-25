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

// https://github.com/atcoder/live_library/blob/master/fps.cpp
// Formal Power Series
using vm = vector<mint>;
struct fps : vm {
#define d (*this)
#define s int(vm::size())
  template<class...Args> fps(Args...args): vm(args...) {}
  fps(initializer_list<mint> a): vm(a.begin(),a.end()) {}
  void rsz(int n) { if (s < n) resize(n);}
  fps& low_(int n) { resize(n); return d;}
  fps low(int n) const { return fps(d).low_(n);}
  mint& operator[](int i) { rsz(i+1); return vm::operator[](i);}
  mint operator[](int i) const { return i<s ? vm::operator[](i) : 0;}
  mint operator()(mint x) const {
    mint r;
    for (int i = s-1; i >= 0; --i) r = r*x+d[i];
    return r;
  }
  fps operator-() const { fps r(d); rep(i,s) r[i] = -r[i]; return r;}
  fps& operator+=(const fps& a) { rsz(a.size()); rep(i,a.size()) d[i] += a[i]; return d;}
  fps& operator-=(const fps& a) { rsz(a.size()); rep(i,a.size()) d[i] -= a[i]; return d;}
  fps& operator*=(const fps& a) { return d = convolution(d, a);}
  fps& operator*=(mint a) { rep(i,s) d[i] *= a; return d;}
  fps& operator/=(mint a) { rep(i,s) d[i] /= a; return d;}
  fps operator+(const fps& a) const { return fps(d) += a;}
  fps operator-(const fps& a) const { return fps(d) -= a;}
  fps operator*(const fps& a) const { return fps(d) *= a;}
  fps operator*(mint a) const { return fps(d) *= a;}
  fps operator/(mint a) const { return fps(d) /= a;}
  fps operator~() const {
    fps r({d[0].inv()});
    for (int i = 1; i < s; i <<= 1) r = r*mint(2) - (r*r*low(i<<1)).low(i<<1);
    return r.low_(s);
  }
  fps& operator/=(const fps& a) { int w = s; d *= ~a; return d.low_(w);}
  fps operator/(const fps& a) const { return fps(d) /= a;}
  fps pow(int t) {
    if (t == 1) return *this;
    fps r = pow(t>>1);
    (r *= r).low_(s);
    if (t&1) (r *= *this).low_(s);
    return r;
  }
  fps diff() const {
    int n = vm::size();
    if (n <= 1) return fps({0});
    fps r(n - 1);
    for (int i = 1; i < n; i++) r[i - 1] = d[i] * i;
    return r;
  }
  fps integ() const {
    fps r;
    rep(i,s) r[i+1] = d[i]/(i+1);
    return r;
  }
  fps log() const {
    assert(d[0].val() == 1); // log(g(x)) は g(0)=1 である必要がある
    int n = vm::size();
    // (log g(x))' = g'(x) / g(x) を積分する
    fps r = (diff() * (~d)).low(n - 1);
    return r.integ().low(n);
  }
  fps exp() const {
    assert(d[0].val() == 0); // exp(f(x)) は f(0)=0 である必要がある
    int n = vm::size();
    fps r({1}); // 初期値は定数項 exp(0) = 1
    for (int i = 1; i < n; i <<= 1) {
      fps l = r.low(i << 1).log();
      fps res = low(i << 1);
      res -= l;
      res[0] += 1;
      r *= res;
      r.low_(i << 1);
    }
    return r.low_(n);
  }

#undef s
#undef d
};
ostream& operator<<(ostream&o,const fps&a) {
  rep(i,a.size()) o<<(i?" ":"")<<a[i].val();
  return o;
}

struct Combination{
    vector<mint> fact, factinv;
    Combination(int n): fact(n+1), factinv(n+1){
        fact[0] = 1;
        for(int i=1; i<=n; i++) fact[i] = fact[i-1]*i;
        factinv[n] = fact[n].inv();
        for(int i=n; i>0; i--) factinv[i-1] = factinv[i]*i;
    }

    mint operator()(int n, int k){
        if(n < 0 || k < 0 || k > n) return 0;
        return fact[n]*factinv[k]*factinv[n-k];
    }

    mint power(mint a, ll b){
        mint res = 1;
        for(; b; b>>=1, a*=a) if(b&1) res *= a;
        return res;
    }
};


void solve()
{
    int n; cin >> n;
    vector<int> ps;
    {
        vector<int> used(n+1, false);
        for (int i = 2; i <= n; i++)
        {
            if (used[i]) continue;
            for (int j = i; j <= n; j += i) used[j] = true;
            ps.emplace_back(i);
        }
    }
    fps h; h.rsz(n+1); h[0] = 1;
    Combination C(n);
    rep(i, ps.size()) h[ps[i]] = C.factinv[ps[i]];
    fps g = h.log();
    g *= mint(n);
    g = g.exp();
    mint ans = g[n-1]*C.fact[n-1]/n;
    cout << ans.val() << "\n";

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