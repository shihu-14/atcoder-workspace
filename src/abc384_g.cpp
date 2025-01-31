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
#define popcountll __builtin_popcountll
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
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

template<typename T=int>
struct CC {
    bool initialized;
    vector<T> xs;
    CC(): initialized(false) {}
    void add(T x) { xs.push_back(x);}
    void init() {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(),xs.end()),xs.end());
        initialized = true;
    }
    int operator()(T x) {
        if (!initialized) init();
        return upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
    }
    T operator[](int i) {
        if (!initialized) init();
        return xs[i];
    }
		int lower_bd(T x) {
			if (!initialized) init();
			int it = upper_bound(xs.begin(), xs.end(), x) - xs.begin();
			return it;
		}
    int size() {
        if (!initialized) init();
        return xs.size();
    }
};

// add,delの演算の定義
struct D {
  int n; ll d; vector<ll>& a, b; fenwick_tree<ll> fw_ac, fw_as, fw_bc, fw_bs;
  CC<ll> cc_a, cc_b; 
  D(vector<ll>& a, vector<ll>& b): n(a.size()), a(a), b(b), d(0), fw_ac(n), fw_as(n), fw_bc(n), fw_bs(n){
    rep(i, n) cc_a.add(a[i]);
    rep(i, n) cc_b.add(b[i]);
    cc_a.init(); cc_b.init();
  }
	void print(){
		rep(i, n) cout << fw_ac.sum(i, i+1) << " "; cout << endl;
		rep(i, n) cout << fw_as.sum(i, i+1) << " "; cout << endl;
		rep(i, n) cout << fw_bc.sum(i, i+1) << " "; cout << endl;
		rep(i, n) cout << fw_bs.sum(i, i+1) << " "; cout << endl;
		cout << endl;
	}
  // 適宜、補助関数を定義。
  void add(ll X, ll Y, bool flag, int e) {
    if (flag){
			int ida = cc_a(a[X-1]);
			if (Y > 0){
				int idb = cc_b.lower_bd(a[X-1]);
				// if (idb != 0 && idb != cc_b.size()) idb++;
				// cout << ida << "->t" << idb << " " << a[X] << endl;
				ll Bsum = fw_bs.sum(0, n);
				ll C = fw_bc.sum(0, idb);
				ll S = fw_bs.sum(0, idb);
				// cout << flag << ":" << Bsum << " " << C << " " << S << "/ " << ida << " " << idb << "/ " << X << ", " << Y << endl;
				d += (Y*a[X-1]+Bsum -2*((Y-C)*a[X-1]+S))*e;
				// cout << "d->" << d << endl;
			}
      fw_ac.add(ida, e);
      fw_as.add(ida, a[X-1]*e);
    }
    else{
			int idb = cc_b(b[Y-1]);
			if (X > 0){
				int ida = cc_a.lower_bd(b[Y-1]);
				// if (ida != 0 && ida != cc_a.size()/) ida++;
				// cout << ida << "->f" << idb << endl;
				ll Asum = fw_as.sum(0, n);
				ll C = fw_ac.sum(0, ida);
				ll S = fw_as.sum(0, ida);	
				// cout << flag << ":" << Asum << " " << C << " " << S << "/ " << ida << " " << idb << "/ " << X << ", " << Y << endl;
				d += (X*b[Y-1]+Asum -2*((X-C)*b[Y-1]+S))*e;
				// cout << "d->" << d << endl;
			}
			fw_bc.add(idb, e);
			fw_bs.add(idb, b[Y-1]*e);
    }
		// print();
  }
  void del(ll X, ll Y, bool flag){
    add(X, Y, flag, -1); 
  }
  ll get() {return d;}
};
// Mo's Algorithmのためのデータの加工と実行。クエリは[l, r)
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
		// cout << nl << ", " << nr << endl;
    while (r < nr) d.add(l, ++r, false, 1);
    while (l < nl) d.add(++l, r, true, 1);
    while (l > nl) d.del(l--, r, true);
    while (r > nr) d.del(l, r--, false);
    res[i] = d.get();
  }
  return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n;
    vector<ll> a(n), b(n);
		rep(i, n) cin >> a[i];
		rep(i, n) cin >> b[i];
    cin >> k;
    vector<pii> query;
    rep(i, k){
      int x, y; cin >> x >> y;
      query.emplace_back(x, y);
    }  
		D d(a, b);
    auto res = Mo(query, d);
    for(auto x: res) cout << x << endl;
    return 0;
}