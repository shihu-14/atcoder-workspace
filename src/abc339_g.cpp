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
template <typename T, T (*op)(T, T), T (*e)()>
struct SegmentTree{
  private:
    int n;
    vector<vector<ll>> data, s;
    void thrust(int i, T x){
        i += n-1;
        data[i].emplace_back(x);
        s[i].emplace_back(x);
        while (i>0){
            i = (i-1)/2;
            data[i].emplace_back(x);
            s[i].emplace_back(x);
        }
    }
    T query(int l, int r, int k, int L, int R, int x){
        if (r <= L || R <= l) return e();
        if (l <= L && R <= r){
            int it = upper_bound(rng(data[k]), x) - data[k].begin();
            return s[k][it];
        }
        T vl = query(l, r, k*2+1, L, (L+R)/2, x);
        T vr = query(l, r, k*2+2, (L+R)/2, R, x);
        return op(vl, vr);
    }
  public:
    SegmentTree(int _n){
        int x = 1;
        while (x < _n) x *= 2;
        n = x;
        data.resize(2*n-1);
        s.resize(2*n-1, {0});
    }
    void build(){
        rep(i, 2*n-1) sort(rng(data[i])), sort(rng(s[i]));
        rep(i, 2*n-1) rep(j, s[i].size()-1) s[i][j+1] += s[i][j];
    }
    void set(int i, T x){ // a[i] = x
        assert(0 <= i && i < n);
        thrust(i, x);
    }
    T prod(int l, int r, int x){ // op(a[l], a[l+1], ..., a[r-1])
        assert(0 <= l && l <= r && r <= n);
        return query(l, r, 0, 0, n, x);
    }
    void print(){
        rep(i, 2*n-1){
            printv(data[i]);
        }
        cout << endl;
        rep(i, 2*n-1){
            printv(s[i]);
        }
    }
};
ll op(ll a, ll b) {return a+b;}
ll e(){return 0;}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n;
    SegmentTree<ll, op, e> seg(n);
    rep(i, n){
        int a; cin >> a;
        seg.set(i, a);
    }
    seg.build();
    // seg.print();
    cin >> q;
    ll pre = 0;
    rep(qi, q){
        ll alpha, beta, gamma; cin >> alpha >> beta >> gamma;
        ll l = alpha^pre, r = beta^pre, x = gamma^pre; l--;
        ll ans = seg.prod(l, r, x);
        cout << ans << '\n';
        pre = ans;
    }
    cout.flush();
    return 0;
}