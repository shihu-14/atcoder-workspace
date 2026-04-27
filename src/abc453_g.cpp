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

#pragma once



template <typename T, typename F, int NODES = 20000000>
struct PersistentSegmentTree {
  using ll = long long;
  struct Node {
    T data;
    Node *l, *r;
    Node() {}
    Node(const T &_data) : data(_data), l(nullptr), r(nullptr) {}
  };

  Node *pool;
  int pid;
  ll N;
  const F f;
  const T ID;
  Node *nil;
  vector<Node *> roots;

  PersistentSegmentTree(const vector<T> &v, const F &f_, const T &ID_)
      : pid(0), f(f_), ID(ID_), nil(nullptr) {
    pool = new Node[NODES];
    nil = my_new(ID);
    nil->l = nil->r = nil;
    roots.reserve(262144);
    roots.push_back(build(v));
  }

  PersistentSegmentTree(const ll &N_, const F &f_, const T &ID_)
      : pid(0), N(N_), f(f_), ID(ID_), nil(nullptr) {
    pool = new Node[NODES];
    nil = my_new(ID);
    nil->l = nil->r = nil;
    roots.reserve(262144);
    roots.push_back(nil);
  }

  Node *my_new(const T &data) {
    pool[pid].data = data;
    pool[pid].l = pool[pid].r = nil;
    return &(pool[pid++]);
  }

  Node *merge(Node *l, Node *r) {
    pool[pid].data = f(l->data, r->data);
    pool[pid].l = l;
    pool[pid].r = r;
    return &(pool[pid++]);
  }

  Node *build(const vector<T> &v) {
    N = (ll)v.size();
    return build(0, (ll)v.size(), v);
  }

  Node *build(ll l, ll r, const vector<T> &v) {
    if (l + 1 == r) return my_new(v[l]);
    ll m = (l + r) >> 1;
    return merge(build(l, m, v), build(m, r, v));
  }

 private:
  Node *update_(ll a, const T &x, Node *n, ll l, ll r) {
    if (l + 1 == r) return my_new(x);
    ll m = (l + r) >> 1;
    if (a < m) return merge(update_(a, x, n->l, l, m), n->r);
    return merge(n->l, update_(a, x, n->r, m, r));
  }
  Node *add_(ll a, const T &x, Node *n, ll l, ll r) {
    if (l + 1 == r) return my_new(f(x, n->data));
    ll m = (l + r) >> 1;
    if (a < m) return merge(add_(a, x, n->l, l, m), n->r);
    return merge(n->l, add_(a, x, n->r, m, r));
  }
  T query_(ll a, ll b, Node *n, ll l, ll r) {
    if (n == nil) return ID;
    if (r <= a or b <= l) return ID;
    if (a <= l and r <= b) return n->data;
    ll m = (l + r) >> 1;
    return f(query_(a, b, n->l, l, m), query_(a, b, n->r, m, r));
  }

 public:
  Node *update(Node *n, ll k, const T &x) {
    Node *root = update_(k, x, n, 0, N);
    roots.push_back(root);
    return root;
  }
  Node *update(int t, ll k, const T &x) {
    Node *root = update_(k, x, roots[t], 0, N);
    roots.push_back(root);
    return root;
  }
  Node *update(ll k, const T &x) {
    Node *root = update_(k, x, roots.back(), 0, N);
    roots.push_back(root);
    return root;
  }

  Node *add(Node *n, ll k, const T &x) {
    Node *root = add_(k, x, n, 0, N);
    roots.push_back(root);
    return root;
  }
  Node *add(int t, ll k, const T &x) {
    Node *root = add_(k, x, roots[t], 0, N);
    roots.push_back(root);
    return root;
  }
  Node *add(ll k, const T &x) {
    Node *root = add_(k, x, roots.back(), 0, N);
    roots.push_back(root);
    return root;
  }

  T query(Node *n, ll a, ll b) { return query_(a, b, n, 0, N); }
  T query(int t, ll a, ll b) { return query_(a, b, roots[t], 0, N); }
  T query(ll a, ll b) { return query_(a, b, roots.back(), 0, N); }

  Node *new_tree() { return nil; }
};

/**
 * @brief 永続セグメント木
 * @docs docs/segment-tree/persistent-segtree.md
 */

ll op(ll a, ll b)
{
    return a+b;
}

void solve()
{
    int n, m, q; cin >> n >> m >> q;
    using P = PersistentSegmentTree<ll, decltype(&op)>;
    P seg(m, op, 0);
    vector<P::Node*> a(n, seg.new_tree());
    
    rep(qi, q)
    {
        int type; cin >> type;
        if (type == 1)
        {
            int x, y; cin >> x >> y; x--, y--;
            a[x] = a[y];
        }
        else if (type == 2)
        {
            int x, y, z; cin >> x >> y >> z; x--, y--;
            a[x] = seg.update(a[x], y, z);
        }
        else
        {
            int x, l, r; cin >> x >> l >> r; x--, l--;
            cout << seg.query(a[x], l, r) << endl;
        }
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