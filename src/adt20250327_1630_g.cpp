// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
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
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

// https://ei1333.github.io/library/structure/segment-tree/dual-segment-tree.hpp.html
template <typename Act>
struct DualSegmentTree {
  using F = typename Act::F;

 private:
  int sz, height;
  vector<F> lazy;
  Act m;

  inline void propagate(int k) {
    if (lazy[k] != m.id()) {
      lazy[2 * k + 0] = m.composition(lazy[2 * k + 0], lazy[k]);
      lazy[2 * k + 1] = m.composition(lazy[2 * k + 1], lazy[k]);
      lazy[k] = m.id();
    }
  }

  inline void thrust(int k) {
    for (int i = height; i > 0; i--) propagate(k >> i);
  }

 public:
  DualSegmentTree(Act m, int n) : m(m) {
    sz = 1;
    height = 0;
    while (sz < n) sz <<= 1, height++;
    lazy.assign(2 * sz, m.id());
  }

  void set(int k, const F &f) {
    thrust(k += sz);  
    lazy[k] = f;    
  }

  F get(int k) {
    thrust(k += sz);
    return lazy[k];
  }

  F operator[](int k) { return get(k); }

  void apply(int a, int b, const F &f) {
    thrust(a += sz);
    thrust(b += sz - 1);
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) lazy[l] = m.composition(lazy[l], f), ++l;
      if (r & 1) --r, lazy[r] = m.composition(lazy[r], f);
    }
  }
};

struct Act{
  // Edit here
  using F = ll;
  static constexpr F composition(const F &f, const F &g) {return f+g;}
  static constexpr F id() {return 0;}
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<ll> a(n);
    rep(i, n) cin >> a[i];
    DualSegmentTree<Act> seg(Act(), n);
    rep(i, n) seg.set(i, a[i]);
    rep(i, n-1){
        ll t = min(seg.get(i), n-i-1);
        seg.apply(i+1, i+1+t, 1);
        seg.set(i, seg.get(i)-t);
    }
    rep(i, n) cout << seg[i] << " ";
    cout << endl;
    return 0;
}