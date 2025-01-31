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
using T = tuple<int, int, int>;
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

struct bit_Matrix {
  using BS = bitset<64>; // Edit bitsize (default 64)
  int h, w;
  vector<BS> d;
  vector<vector<BS>> exp_d;
  bit_Matrix(int h, int w): h(h), w(w), d(h), exp_d(64, vector<BS>(h)){}
  BS operator[](int i) const {return d[i];}
  BS& operator[](int i) {return d[i];}
  void build(){ // build for using power()
    assert(h == w); int n = h;
    exp_d[0] = d;
    vector<BS> d_trans(n);
    rep(i, n)rep(j, n) d_trans[i][j] = d[j][i];
    rep2(t, 1, 64){
      vector<BS> nxt_d_trans(n);
      rep(i, n)rep(j, n){
        exp_d[t][i][j] = (exp_d[t-1][i]&d_trans[j]).any() ;
        nxt_d_trans[i][j] = (d_trans[i]&exp_d[t-1][j]).any();
      }
      d_trans = nxt_d_trans;
    }
  }
  BS power(BS x, ll n){ // calc d^(n)*x
    assert(h == w); 
    BS res=x;
    for (int i = 0; n; n >>= 1, i++)if(n&1){
      rep(j, h) res[j] = (exp_d[i][j]&x).any();
      x = res;
    }
    return res;
  }
  BS solve() { // treat matrix 'd' as augmented coefficient matrix and solve
    int ni = 0;
    rep(j,w-1) {
      for (int i = ni; i < h; i++) {
        if (d[i][j]) {swap(d[i], d[ni]); break;}
      }
      if (!d[ni][j]) continue;
      rep(i,h) if (ni != i && d[i][j]) d[i] ^= d[ni];
      ni++; if (ni == h) break;
    }
    BS nan, res; nan[w-1] = 1;
    rep(i,h) {
      int j = 0;
      while (j < w-1 && !d[i][j]) j++;
      if (j == w-1) {
        if (d[i][w-1]) return nan;
        continue;
      }
      res[j] = d[i][w-1];
    }
    return res;
  }
};
ostream& operator<<(ostream&o,const bit_Matrix&a) {
  rep(i,a.h) o<<a[i]<<endl;
  return o;
}

int main(){
    int n, m; cin >> n >> m;
    bit_Matrix a(n+1, n+1);
    rep(i, m){
        int u, v; cin >> u >> v; u--, v--;
        a[u][v] = a[v][u] = 1;
    }
    vector<ll> ans(n);
    rep(i, n){
        bit_Matrix na = a; 
        na[n][i] = 1, na[n][n] = 1;
        auto res = na.solve();
        if (res[n] == 1){
            cout << "No" << endl;
            return 0;
        }
        rep(j, n) if (res[j]) ans[j] |= 1LL<<i;
    }
    cout << "Yes" << endl;
    printv(ans);
    return 0;
}