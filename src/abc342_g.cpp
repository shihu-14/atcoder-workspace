// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int, int>;
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
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;

// https://www.youtube.com/live/n7WZOQVuGp4
// Deletable Dual Segment Tree
struct DualSegmentTree2 {
  int n;
  vector<multiset<int>> d; 
  DualSegmentTree2(int mx=0) {
    n = 1;
    while (n < mx) n <<= 1;
    d.resize(n*2);
  }
  void add(int l, int r, int x) {
    l += n; r += n;
    while (l < r) {
      if (l%2) d[l].insert(x), l++;
      if (r%2) r--, d[r].insert(x);
      l >>= 1; r >>= 1;
    }
  }
  void del(int l, int r, int x) {
    l += n; r += n;
    while (l < r) {
      if (l%2) d[l].erase(d[l].find(x)), l++;
      if (r%2) r--, d[r].erase(d[r].find(x));
      l >>= 1; r >>= 1;
    }
  }
  int get(int i) {
    int res = 0;
    i += n;
    while (i) {
      if (d[i].size()) res = max(res, *d[i].rbegin()); // Edit here
      i >>= 1;
    }
    return res;
  }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    DualSegmentTree2 seg(n);
    rep(i, n) seg.add(i, i+1, a[i]);
    cin >> q;
    vector<T> query(q);
    rep(qi, q){
        int type; cin >> type;
        if (type == 1){
            int l, r, x; cin >> l >> r >> x; l--;
            seg.add(l, r, x);
            query[qi] = {type, l, r, x};
        }
        else if (type == 2){
            int id; cin >> id; id--;
            auto [_, l, r, x] = query[id];
            seg.del(l, r, x);
        }
        else{
            int id; cin >> id; id--;
            cout << seg.get(id) << endl;
        }
    }
    return 0;
}