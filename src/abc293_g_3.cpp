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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

const int MX = 200010;
// add,delの演算の定義
struct D {
  int n; ll d; vector<int>& a; vector<int> cnt; // 必要に応じてデータ構造を追加。
  D(vector<int>& a): n(a.size()), a(a), cnt(MX), d(0) {}
  ll f(ll x){ return x*(x-1)/2*(x-2)/3;}
  void add(int i) {
    // ここに処理を加える
    d -= f(cnt[a[i]]);
    cnt[a[i]]++;
    d += f(cnt[a[i]]);
  }
  void del(int i){
    // ここに処理を加える
    d -= f(cnt[a[i]]);
    cnt[a[i]]--;
    d += f(cnt[a[i]]);
  }
  ll get() {return d;}
};
// Mo's Algorithmのためのデータの加工と実行 [l, r)
template<class T=long long>
vector<T> Mo(vector<pair<int,int>>& query, D& d) {
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
    res[i] = d.get();
  }
  return res;
}
int main() {
  int n, q;
  cin >> n >> q;
  vector<int> a(n);
  rep(i,n) cin >> a[i];
  vector<pii> query;
  rep(i, q){
    int l, r; cin >> l >> r; l--;
    query.emplace_back(l, r);
  }
  D d(a);
  auto ans = Mo(query, d);
  rep(i, q) cout << ans[i] << endl;
  return 0;
}