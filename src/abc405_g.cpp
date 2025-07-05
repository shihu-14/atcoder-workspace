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

const int MX = 200010;
// add,delの演算の定義
struct D {
    int n, M; vector<int> &a, cnt1, cnt1m; // 必要に応じてデータ構造を追加。
    vector<mint> fact, factinv, cnt2, cnt2m;
    D(vector<int>& a): n(a.size()), a(a), M(sqrt(n)+1), cnt1m(n/M+1), cnt2m(n/M+1,1), cnt1(n), cnt2(n,1), fact(n+1, 1), factinv(n+1, 1){
        rep(i, n) fact[i+1] = fact[i]*(i+1);
        rep(i, n+1) factinv[i] = fact[i].inv();
    }
    // 適宜、補助関数を定義。
    
    void add(int i) {
      // ここに処理を加える
      int v = a[i], j = v/M;
      cnt1[v]++;
      cnt1m[j]++;
      cnt2[v] *= cnt1[v];
      cnt2m[j] *= factinv[cnt1[v]-1];
      cnt2m[j] *= fact[cnt1[v]];
    }
    void del(int i){
      // ここに処理を加える
      int v = a[i], j = v/M;
      cnt1[v]--;
      cnt1m[j]--;
      cnt2[v] = fact[cnt1[v]];
      cnt2m[j] *= factinv[cnt1[v]+1];
      cnt2m[j] *= fact[cnt1[v]];
    }
    mint get(int x) {
        int tot = 0;
        mint res = 1;
        for (int i=0, si=0; i<x;){
            if (i+M <= x){
                tot += cnt1m[si];
                res *= cnt2m[si];
                i += M;
                si++;
            }
            else{
                tot += cnt1[i];
                res *= cnt2[i];
                i++;
            }
        }
        return fact[tot]/res;
    }
  };
  // Mo's Algorithmのためのデータの加工と実行。クエリは[l, r)
  template<class T=long long>
  vector<T> Mo(vector<pair<int,int>>& query, D& d, vector<int> x) {
    int Q = query.size(); 
    vector<T> res(Q); // クエリに対する答えを保持
    int W = d.n/(sqrt(Q)+1)+1; // 0除算防止の+1
    vector<int> id(Q); iota(id.begin(), id.end(), 0); // 何番目のクエリかを保持
    vector<int> Wid(Q); for(int i=0; i<Q; ++i) Wid[i] = query[i].first/W; // あるクエリがどの縦区分に属するかを保持
    sort(id.begin(), id.end(), [&](int i, int j) { // 実行クエリをソートするための基準を決定
      if (Wid[i] != Wid[j]) return Wid[i] < Wid[j]; // 区分が異なるなら昇順。
      if (Wid[i]&1) return query[i].second > query[j].second; // 区分が同じで奇数番目であれば、rが降順になるように
      else return query[i].second < query[j].second; // 偶数番目ならば昇順になるようにする。(区分を跨ぐ時に最小のコストで移動するため)
    });
    int l = 0, r = 0;
    for (int i : id) {
      auto [nl, nr] = query[i];
      while (r < nr) d.add(r++);
      while (l > nl) d.add(--l);
      while (l < nl) d.del(l++);
      while (r > nr) d.del(--r);
      res[i] = d.get(x[i]);
    }
    return res;
  }
  

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    rep(i, n) cin >> a[i], a[i]--;
    vector<pii> query;
    vector<int> X(q);
    rep(qi, q){
        int l, r, x; cin >> l >> r >> x; l--; x--;
        query.emplace_back(l, r);
        X[qi] = x;
    }
    D d(a);
    auto ans = Mo<mint>(query, d, X);
    rep(i, q) cout << ans[i].val() << endl;
    return 0;
}